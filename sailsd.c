#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <getopt.h>


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

static void log_msg(const char *level, const char *format, ...) {
    va_list arglist;
    va_start(arglist, format);

    printf("%s: ", level);
    vprintf(format, arglist);
    printf("\n");
    va_end(arglist);
}

int main(int argc, char *argv[]) {
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
                return 0;
            default:
                return 0;
        }
    }

    put_boat();
    log_msg("info", "started sailsd");
    return 0;
}
