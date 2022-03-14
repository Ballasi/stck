// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef __CMD_NEWLINE_H
#define __CMD_NEWLINE_H

#include "stack.h"
#include <stdio.h>
#include <string.h>

#define CMD_NEWLINE {cmd_newline_test, cmd_newline_function}

static const int cmd_newline_test(char *action, int size) {
  return !strcmp(action, "\\n");
}

static const void cmd_newline_function(Stack** buffer) {
  free(Stack_pop(buffer));
  char *value = (char *)Stack_pop(buffer);
  char *ptr = value;
  if (ptr[0] == '\\')
    ++ptr;
  printf("%s\n", ptr);
  free(value);
}

#endif // __CMD_NEWLINE_H