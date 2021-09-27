// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef __STACK_H
#define __STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Stack {
  void *val;
  struct Stack *next;
  int val_size;
  int stack_size;
} Stack;

typedef struct Value {
  void *val;
  int size;
} Value;

void Stack_leaf(Stack **list);
void Stack_new(Stack **list, void *val, int size);

void Stack_push(Stack **list, void *val, int size);
void *Stack_pop(Stack **list);

int Stack_isEmpty(Stack **list);
int Stack_size(Stack **list);
void Stack_headValue(Stack **list, Value *value);
void Stack_printString(Stack **list, int comma);

#endif // __STACK_H
