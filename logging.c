#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "logging.h"

enum log_level { ERROR, WARNING, INFO, DEBUG };

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

void log_info(const char *format, ...) {
    va_list arglist;
    va_start(arglist, format);
    vlog_msg(INFO, format, arglist);
    va_end(arglist);
}

void log_error(const char *format, ...) {
    va_list arglist;
    va_start(arglist, format);
    vlog_msg(ERROR, format, arglist);
    va_end(arglist);
}

void log_warning(const char *format, ...) {
    va_list arglist;
    va_start(arglist, format);
    vlog_msg(WARNING, format, arglist);
    va_end(arglist);
}

void log_debug(const char *format, ...) {
    va_list arglist;
    va_start(arglist, format);
    vlog_msg(DEBUG, format, arglist);
    va_end(arglist);
}
