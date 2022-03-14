// SPDX-License-Identifier: GPL-3.0-or-later

#include "command.h"
#include "commands/concat.h"
#include "commands/escaping.h"
#include "commands/newline.h"
#include "commands/print.h"
#include "commands/stck.h"
#include "commands/variable_assign.h"
#include "commands/variable_call.h"

#include <stdio.h>

const Command commands[] = {
    CMD_NEWLINE, CMD_ESCAPING, CMD_VARIABLE_CALL, CMD_VARIABLE_ASSIGN,
    CMD_PRINT,   CMD_STCK,     CMD_CONCAT,        CMD_NULL};

void handleCommands(Stack **buffer) {
  Value action_value;
  Stack_get(buffer, &action_value, 0);
  char *action = (char *)action_value.val;
  int size = Stack_size(buffer);
  for (int i = 0; commands[i].test; ++i) {
    Command c = commands[i];
    if (c.test(action, size)) {
      c.function(buffer);
      return;
    }
  }
}
