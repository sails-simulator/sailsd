#ifndef SAILSD_LOGGING_H
#define SAILSD_LOGGING_H

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

void log_info(const char *format, ...);
void log_error(const char *format, ...);
void log_warning(const char *format, ...);
void log_debug(const char *format, ...);

#endif
