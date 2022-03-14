// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef __CMD_ESCAPING_H
#define __CMD_ESCAPING_H

#include "stack.h"
#include <stdio.h>
#include <string.h>

#define CMD_ESCAPING { cmd_escaping_test, cmd_escaping_function }

static const int cmd_escaping_test(char *action, int size) {
  return action[0] == '\\';
}

static const void cmd_escaping_function(Stack **buffer) {
  // don't do anything
}

#endif // __CMD_ESCAPING_H
