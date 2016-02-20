/* This file is part of sailsd
 *
 * Copyright (C) 2014-2016 Louis Taylor <louis@kragniz.eu>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>
#include <resolv.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <getopt.h>

#include <jansson.h>

#define SAILSD_PORT 3333
#define SAILSD_MAX_MESSAGE_LENGTH 2048
#define SAILSD_VERSION "1.0"

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

/* flag used by sigint signal handling. If set to 1, sailsd should quit */
volatile sig_atomic_t quitting_flag = 0;

/* bitmask for attributes */
enum request_attribute_t {
    REQUEST_VERSION = 0x01,
    REQUEST_STATE   = 0x02,
};

enum log_level { ERROR, WARNING, INFO, DEBUG };

struct request_t {
    bool error;
    /* bitmask of REQUEST_VERSION etc values */
    int requested_attributes;
};

/* print a giant boat to the screen */
void put_boat(void) {
    puts("                            \n"
         "                ,\x1b[31m~\x1b[0m          \n"
         "                |\\         \n"
         "               /| \\        \n"
         "         \x1b[34m~ ^~ \x1b[0m/_|__\\\x1b[34m~^~    \x1b[0m\n"
         "      \x1b[34m~^~^ ~ \x1b[0m'======' \x1b[34m~^ ~^~  \x1b[0m\n"
         "    \x1b[34m~^  ^~ ~^ ~\x1b[0m_\x1b[34m~\x1b[0m_\x1b[34m~^~ ~^~\x1b[0m_\x1b[34m ~^~  \x1b[0m\n"
         "   \x1b[34m~^~ \x1b[0m___\x1b[34m^\x1b[0m___|_| |___\x1b[34m~\x1b[0m_| | \x1b[34m ^~  \x1b[0m\n"
         "    \x1b[34m~ \x1b[0m|_ -| .'| | |_ -| . |\x1b[34m ~ \x1b[0m\n"
         "      |___|__,|_|_|___|___| \n");
}

/* logging function in the vein of vprintf, taking a va_list of arguments */
static void vlog_msg(const enum log_level level,
                     const char *format,
                     va_list argp) {
    char *level_str = "";
    switch(level) {
        case ERROR:
            level_str = COLOR_RED "error" COLOR_RESET;
            break;
        case WARNING:
            level_str = COLOR_YELLOW "warning" COLOR_RESET;
            break;
        case INFO:
            level_str = COLOR_BLUE "info" COLOR_RESET;
            break;
        case DEBUG:
            level_str = "debug";
            break;
    }

    char timestamp[32];

    time_t t = time(NULL);
    struct tm *p = localtime(&t);
    strftime(timestamp, 32, "%c", p);

    printf("[%s] %s:\t", timestamp, level_str);
    vprintf(format, argp);
    printf("\n");
}

static void log_msg(const enum log_level level, const char *format, ...) {
    va_list arglist;
    va_start(arglist, format);
    vlog_msg(level, format, arglist);
    va_end(arglist);
}

/* TODO: add log_error etc */
static void log_info(const char *format, ...) {
    va_list arglist;
    va_start(arglist, format);
    vlog_msg(INFO, format, arglist);
    va_end(arglist);
}

static void log_error(const char *format, ...) {
    va_list arglist;
    va_start(arglist, format);
    vlog_msg(ERROR, format, arglist);
    va_end(arglist);
}

static void log_warning(const char *format, ...) {
    va_list arglist;
    va_start(arglist, format);
    vlog_msg(WARNING, format, arglist);
    va_end(arglist);
}

static void log_debug(const char *format, ...) {
    va_list arglist;
    va_start(arglist, format);
    vlog_msg(DEBUG, format, arglist);
    va_end(arglist);
}

struct request_t *request_t_init(void) {
    struct request_t *r = calloc(1, sizeof(struct request_t));
    r->requested_attributes = 0;
    r->error = false;

    return r;
}

void request_t_add_requested_attribute(struct request_t *r,
                                       const enum request_attribute_t a) {
    r->requested_attributes |= a;
}

struct request_t *parse_request(const char *request_str) {
    json_error_t error;

    json_t *root = json_loads(request_str, 0, &error);

    struct request_t *r = request_t_init();

    if (!root) {
        log_error("request is not valid json at line %d: %s",
                  error.line,
                  error.text);
        goto error;
    }

    if (!json_is_object(root)) {
        log_error("request is not a json object");
        r->error = true;
        goto error;
    }

    json_t *request = json_object_get(root, "request");
    if (!json_is_array(request)) {
        log_error("\"request\" is not a string");
        r->error = true;
        goto error;
    }

    for (int i=0; i<json_array_size(request); i++) {
        const char *val = json_string_value(json_array_get(request, i));
        if (strcmp(val, "version") != 0) {
            request_t_add_requested_attribute(r, REQUEST_VERSION);
        }
    }

error:
    json_decref(root);
    return r;
}

json_t *make_error_resp(char *msg) {
    return json_pack("{ss}", "error", msg);
}

json_t *make_resp(struct request_t *request) {
    json_t *response = json_object();

    if (!request->requested_attributes & REQUEST_VERSION) {
        log_debug("requested the version");
        json_object_set(response, "version", json_string(SAILSD_VERSION));
    }
    return response;
}

void *worker(void *arg) {
    char *line = calloc(1, SAILSD_MAX_MESSAGE_LENGTH);
    int bytes_read;
    int client = *(int *)arg;

    log_debug("started thread");

    bytes_read = recv(client, line, SAILSD_MAX_MESSAGE_LENGTH, 0);
    if (bytes_read == -1) {
        perror("error reading from socket");
    } else {
        json_t *resp;

        log_debug("<- " COLOR_CYAN "\"%s\"" COLOR_RESET, line);
        log_debug("bytes read: %i", bytes_read);

        struct request_t *r = parse_request(line);
        if (r->error) {
            log_warning("error in request");
            resp = make_error_resp("you messed up");
        } else {
            resp = make_resp(r);
        }
        char *resp_str = json_dumps(resp, 0);
        log_debug("-> " COLOR_CYAN "\"%s\"" COLOR_RESET, resp_str);
        send(client, resp_str, strlen(resp_str), 0);
        free(r);
        free(resp);
    }

    /* clean up and return */
    close(client);
    free(line);
    log_debug("closed thread");
    return arg;
}

void sigint_handler(int sig) {
    log_debug("got signal %i", sig);
    quitting_flag = 1;
    exit(1);
}

int main(int argc, char *argv[]) {
    int c;
    const char *short_opt = "h";
    struct option long_opt[] = {
        {"help", no_argument, NULL, 'h'},
        {NULL,   0,           NULL, 0  }
    };

    /* register signal handler to catch C-c signals */
    signal(SIGINT, sigint_handler);

    while((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
        switch(c) {
            case -1: /* no more arguments */
            case 0:  /* long options toggles */
                break;
            case 'h':
                printf("Usage: %s [OPTIONS]\n", argv[0]);
                printf("  -h, --help            print this help and exit\n");
                printf("\n");
                return 0;
            default:
                return 0;
        }
    }

    put_boat();
    log_info("started sailsd");
    log_msg(WARNING, "warning log");
    log_warning("warning log");

    /* start up socket server */
    struct sockaddr_in addr;

    int sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        log_msg(ERROR, "cannot start socket");
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SAILSD_PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        log_msg(ERROR, "failed to listen on port");
        perror("failed to listen on port");
        exit(1);
    }

    if (listen(sd, 20) != 0) {
        log_msg(ERROR, "failed to listen on port");
    }
    log_info("listening on port %i", SAILSD_PORT);

    while (!quitting_flag) {
        socklen_t addr_size = sizeof(addr);
        pthread_t child;

        int client = accept(sd, (struct sockaddr*)&addr, &addr_size);
        log_info("connected: %s:%d",
                 inet_ntoa(addr.sin_addr),
                 ntohs(addr.sin_port));
        if (pthread_create(&child, NULL, worker, &client) != 0) {
            perror("error creating thread");
        } else {
            pthread_detach(child);
        }
    }

    return 0;
}
