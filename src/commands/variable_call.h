// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef __CMD_VARIABLE_CALL_H
#define __CMD_VARIABLE_CALL_H

#include "stack.h"
#include "variable.h"
#include <stdio.h>
#include <string.h>

#define CMD_VARIABLE_CALL {cmd_variable_call_test, cmd_variable_call_function}

static const int cmd_variable_call_test(char *action, int size) {
  return action[0] == '~';
}

static const void cmd_variable_call_function(Stack** buffer) {
  Value action_value;
  Stack_get(buffer, &action_value, 0);
  char *action = (char *)action_value.val;

  char *value;
  if ((value = Variable_get(action + 1))) {
    free(Stack_pop(buffer));
    Stack_push(buffer, value, strlen(value) + 1);
  }
}

#endif // __CMD_VARIABLE_CALL_H