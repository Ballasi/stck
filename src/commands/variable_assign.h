// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef __CMD_VARIABLE_ASSIGN_H
#define __CMD_VARIABLE_ASSIGN_H

#include "stack.h"
#include "variable.h"
#include <stdio.h>
#include <string.h>

#define CMD_VARIABLE_ASSIGN {cmd_variable_assign_test, cmd_variable_assign_function}

static const int cmd_variable_assign_test(char *action, int size) {
  return size >= 3 && action[0] == '=' && action[1] == 0;
}

static const void cmd_variable_assign_function(Stack** buffer) {
  free(Stack_pop(buffer));
  char *name = (char *)Stack_pop(buffer);
  char *value = (char *)Stack_pop(buffer);
  Variable_put(name, value);
}

#endif // __CMD_VARIABLE_ASSIGN_H
