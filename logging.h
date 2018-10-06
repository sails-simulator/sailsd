/* This file is part of sailsd
 *
 * Copyright (C) 2014-2018 Louis Taylor <louis@kragniz.eu>
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

#ifndef SAILSD_LOGGING_H
#define SAILSD_LOGGING_H

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

enum log_level { ERROR, WARNING, INFO, DEBUG };

extern enum log_level current_log_level;

void put_boat(void);
void log_info(const char *format, ...);
void log_error(const char *format, ...);
void log_warning(const char *format, ...);
void log_debug(const char *format, ...);

#endif
