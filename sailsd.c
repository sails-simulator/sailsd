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

/* FIXME: remove */
#include <limits.h>

#include <jansson.h>
#include <sailing.h>

#include "logging.h"

#define SAILSD_PORT 3333
#define SAILSD_MAX_MESSAGE_LENGTH 2048
#define SAILSD_VERSION "1.0"

/* flag used by sigint signal handling. If set to 1, sailsd should quit */
volatile sig_atomic_t quitting_flag = 0;

/* bitmask for attributes */
enum request_attribute {
    REQUEST_VERSION      = 0x001,
    REQUEST_STATE        = 0x002,
    REQUEST_LATITUDE     = 0x004,
    REQUEST_LONGITUDE    = 0x008,
    REQUEST_SAIL_ANGLE   = 0x010,
    REQUEST_HEADING      = 0x020,
    REQUEST_RUDDER_ANGLE = 0x040,
    REQUEST_WIND_SPEED   = 0x080,
    REQUEST_WIND_ANGLE   = 0x100,
    REQUEST_SPEED        = 0x200,
};

struct request_t {
    bool error;
    /* bitmask of REQUEST_VERSION etc values */
    int requested_attributes;
};

struct state {
    bool running;
    Boat *boat;
    Wind *wind;
    pthread_mutex_t physics_mutex;
};

/* state singleton */
struct state *world_state;

/* TODO: write state_free() */
struct state *state_init(void) {
    struct state *state = calloc(1, sizeof(struct state));
    state->running = false;
    state->boat = sailing_boat_init();
    state->wind = sailing_wind_new();
    pthread_mutex_init(&state->physics_mutex, NULL);

    return state;
}

void state_set_running(struct state *state, bool value) {
    state->running = value;
}

struct request_t *request_t_init(void) {
    struct request_t *r = calloc(1, sizeof(struct request_t));
    r->requested_attributes = 0;
    r->error = false;

    return r;
}

/* FIXME: remove this function */

char * int2bin(int i) {
    size_t bits = sizeof(int) * CHAR_BIT;

    char * str = malloc(bits + 1);
    if(!str) return NULL;
    str[bits] = 0;

    // type punning because signed shift is implementation-defined
    unsigned u = *(unsigned *)&i;
    for(; bits--; u >>= 1)
        str[bits] = u & 1 ? '1' : '0';

    return str;
}

void request_t_add_requested_attribute(struct request_t *r,
                                       const enum request_attribute a) {
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

    /* check that "request" is an array */
    json_t *request = json_object_get(root, "request");
    if (json_is_array(request)) {
        /* check each requested attribute named in the array */
        for (int i=0; i<json_array_size(request); i++) {
            const char *val = json_string_value(json_array_get(request, i));
            if (strcmp(val, "version") == 0) {
                request_t_add_requested_attribute(r, REQUEST_VERSION);
            } else if (strcmp(val, "latitude") == 0) {
                request_t_add_requested_attribute(r, REQUEST_LATITUDE);
            } else if (strcmp(val, "longitude") == 0) {
                request_t_add_requested_attribute(r, REQUEST_LONGITUDE);
            } else if (strcmp(val, "sail-angle") == 0) {
                request_t_add_requested_attribute(r, REQUEST_SAIL_ANGLE);
            } else if (strcmp(val, "heading") == 0) {
                request_t_add_requested_attribute(r, REQUEST_HEADING);
            } else if (strcmp(val, "rudder-angle") == 0) {
                request_t_add_requested_attribute(r, REQUEST_RUDDER_ANGLE);
            } else if (strcmp(val, "wind-speed") == 0) {
                request_t_add_requested_attribute(r, REQUEST_WIND_SPEED);
            } else if (strcmp(val, "wind-angle") == 0) {
                request_t_add_requested_attribute(r, REQUEST_WIND_ANGLE);
            } else if (strcmp(val, "speed") == 0) {
                request_t_add_requested_attribute(r, REQUEST_SPEED);
            } else {
                log_warning("requested '%s', which is not a recognized attribute", val);
            }
        }
        log_debug("parse_request bitmask:	'%s'", int2bin(r->requested_attributes));
    }

    /* check that "set" is a object */
    json_t *set = json_object_get(root, "set");
    if (json_is_object(set)) {
        const char *key;
        json_t *value;

        pthread_mutex_lock(&world_state->physics_mutex);

        /* this should probably be batched and handled after the parsing of the request is done */
        json_object_foreach(set, key, value) {
            log_debug("key: %s", key);
            if (strcmp(key, "latitude") == 0) {
                log_debug("setting latitude to %f", json_number_value(value));
                sailing_boat_set_latitude(world_state->boat,
                                          json_number_value(value));
            } else if (strcmp(key, "longitude") == 0) {
                log_debug("setting longitude to %f", json_number_value(value));
                sailing_boat_set_longitude(world_state->boat,
                                           json_number_value(value));
            } else if (strcmp(key, "sail-angle") == 0) {
                sailing_boat_set_sail_angle(world_state->boat,
                                            json_number_value(value));
            } else if (strcmp(key, "heading") == 0) {
                sailing_boat_set_angle(world_state->boat,
                                       json_number_value(value));
            } else if (strcmp(key, "rudder-angle") == 0) {
                sailing_boat_set_rudder_angle(world_state->boat,
                                              json_number_value(value));
            } else if (strcmp(key, "wind-speed") == 0) {
                sailing_wind_set_speed(world_state->wind,
                                       json_number_value(value));
            } else if (strcmp(key, "wind-angle") == 0) {
                sailing_wind_set_direction(world_state->wind,
                                              json_number_value(value));
            } else {
                log_warning("tried to set '%s', which is not a recognized attribute", value);
            }
        }

        pthread_mutex_unlock(&world_state->physics_mutex);
    }

error:
    json_decref(root);
    return r;
}

json_t *make_error_resp(char *msg) {
    return json_pack("{ss}", "error", msg);
}

bool request_attribute_contains(int requested_attributes, enum request_attribute attr) {
    if ((requested_attributes & attr) == attr) {
        return true;
    } else {
        return false;
    }
}

json_t *make_resp(struct request_t *request) {
    json_t *response = json_object();

    if (request_attribute_contains(request->requested_attributes, REQUEST_VERSION)) {
        json_object_set(response,
                        "version",
                        json_string(SAILSD_VERSION));
    }

    if (request_attribute_contains(request->requested_attributes, REQUEST_LATITUDE)) {
        json_object_set(response,
                        "latitude",
                        json_real(sailing_boat_get_latitude(world_state->boat)));
    }

    if (request_attribute_contains(request->requested_attributes, REQUEST_LONGITUDE)) {
        json_object_set(response,
                        "longitude",
                        json_real(sailing_boat_get_longitude(world_state->boat)));
    }

    if (request_attribute_contains(request->requested_attributes, REQUEST_SAIL_ANGLE)) {
        json_object_set(response,
                        "sail-angle",
                        json_real(sailing_boat_get_sail_angle(world_state->boat)));
    }

    if (request_attribute_contains(request->requested_attributes, REQUEST_HEADING)) {
        json_object_set(response,
                        "heading",
                        json_real(sailing_boat_get_angle(world_state->boat)));
    }

    if (request_attribute_contains(request->requested_attributes, REQUEST_RUDDER_ANGLE)) {
        json_object_set(response,
                        "rudder-angle",
                        json_real(sailing_boat_get_rudder_angle(world_state->boat)));
    }

    if (request_attribute_contains(request->requested_attributes, REQUEST_SPEED)) {
        json_object_set(response,
                        "speed",
                        json_real(sailing_boat_get_velocity(world_state->boat)));
    }

    if (request_attribute_contains(request->requested_attributes, REQUEST_WIND_SPEED)) {
        json_object_set(response,
                        "wind-speed",
                        json_real(sailing_wind_get_speed(world_state->wind)));
    }

    if (request_attribute_contains(request->requested_attributes, REQUEST_WIND_ANGLE)) {
        json_object_set(response,
                        "wind-angle",
                        json_real(sailing_wind_get_direction(world_state->wind)));
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

void *simulation_thread(void *arg) {
    /* hardcode a sleep time of half a second (500,000,000 nanoseconds) */
    struct timespec t, t1;
    t.tv_sec  = 0;
    t.tv_nsec = 5000000L;

    for (;;) {
        while(world_state->running) {
            pthread_mutex_lock(&world_state->physics_mutex);
            /*log_debug("simulation looping position (%f, %f)...",
                      sailing_boat_get_latitude(world_state->boat),
                      sailing_boat_get_longitude(world_state->boat));*/

            /* TODO: implement better integration (runge-kutta would be good) */
            for (int i=0; i<10000; i++) {
                sailing_physics_update(world_state->boat, world_state->wind, 0.000002);
            }
            pthread_mutex_unlock(&world_state->physics_mutex);
            nanosleep(&t, &t1);
        }
        nanosleep(&t, &t1);
    }
}

struct sockaddr_in *socket_init(struct sockaddr_in *addr, int *sd) {
    if (*sd < 0) {
        log_error("cannot start socket");
    }

    addr->sin_family = AF_INET;
    addr->sin_port = htons(SAILSD_PORT);
    addr->sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(*sd, (struct sockaddr*)addr, sizeof(*addr)) != 0) {
        log_error("failed to listen on port");
        perror("failed to listen on port");
        exit(1);
    }

    if (listen(*sd, 20) != 0) {
        log_error("failed to listen on port");
    }
    log_info("listening on port %i", SAILSD_PORT);
    return addr;
}

void sigint_handler(int sig) {
    log_debug("got signal %i", sig);
    quitting_flag = 1;
    exit(1);
}

void parse_args(int argc, char *argv[]) {
    int c;
    const char *short_opt = "h";
    struct option long_opt[] = {
        {"help", no_argument, NULL, 'h'},
        {NULL,   0,           NULL, 0  }
    };

    while((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
        switch(c) {
            case -1: /* no more arguments */
            case 0:  /* long options toggles */
                break;
            case 'h':
                printf("Usage: %s [OPTIONS]\n", argv[0]);
                printf("  -h, --help            print this help and exit\n");
                printf("\n");
                exit(0);
            default:
                exit(0);
        }
    }
}

int main(int argc, char *argv[]) {
    /* handle arguments */
    parse_args(argc, argv);

    /* register signal handler to catch C-c signals */
    signal(SIGINT, sigint_handler);

    world_state = state_init();

    put_boat();
    log_info("started sailsd");
    log_warning("warning log");

    /* start simulation thread */
    pthread_t simulation;
    if (pthread_create(&simulation, NULL, simulation_thread, NULL) != 0) {
        perror("error creating thread");
    } else {
        pthread_detach(simulation);
    }

    /* start up socket server */
    struct sockaddr_in addr;
    int sd = socket(PF_INET, SOCK_STREAM, 0);
    socket_init(&addr, &sd);

    /* start running the simulation */
    state_set_running(world_state, true);

    /* loop and spawn a new thread for each socket connection */
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
