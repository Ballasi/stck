// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef __COMMAND_H
#define __COMMAND_H

#include "stack.h"

#define CMD_NULL { NULL, NULL }

typedef struct {
  const int (*test)(char *, int);
  const void (*function)(Stack **);
} Command;

extern const Command commands[];
void handleCommands(Stack **buffer);

#endif // __COMMAND_H
