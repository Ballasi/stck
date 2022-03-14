// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef __CMD_PRINT_H
#define __CMD_PRINT_H

#include "stack.h"
#include <stdio.h>
#include <string.h>

#define CMD_PRINT {cmd_print_test, cmd_print_function}

static const int cmd_print_test(char *action, int size) {
  return size >= 2 && !strcmp(action, "print");
}

static const void cmd_print_function(Stack** buffer) {
  free(Stack_pop(buffer));
  char *value = (char *)Stack_pop(buffer);
  char *ptr = value;
  if (ptr[0] == '\\')
    ++ptr;
  printf("%s\n", ptr);
  free(value);
}

#endif // __CMD_PRINT_H