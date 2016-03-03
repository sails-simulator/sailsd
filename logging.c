#include "logging.h"

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

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
