// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdarg.h>
#include <stdio.h>

void log_warn(char *message, ...) {
  va_list arg;

  va_start(arg, message);
  fprintf(stderr, "  \033[1m\033[93mWarning \033[0m");
  vfprintf(stderr, message, arg);
  fprintf(stderr, "\n");
  va_end(arg);
}

void log_error(char *message, ...) {
  va_list arg;

  va_start(arg, message);
  fprintf(stderr, "    \033[1m\033[91mError \033[0m");
  vfprintf(stderr, message, arg);
  fprintf(stderr, "\n");
  va_end(arg);
}
