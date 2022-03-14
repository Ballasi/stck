// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef __CMD_STCK_H
#define __CMD_STCK_H

#include "stack.h"
#include <stdio.h>
#include <string.h>

#define CMD_STCK {cmd_stck_test, cmd_stck_function}

static const int cmd_stck_test(char *action, int size) {
  return !strcmp(action, "stck");
}

static const void cmd_stck_function(Stack** buffer) {
  free(Stack_pop(buffer));
  printf("[");
  Stack_printString(buffer, 0);
  printf("]\n");
}

#endif // __CMD_STCK_H