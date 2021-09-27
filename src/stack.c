// SPDX-License-Identifier: GPL-3.0-or-later

#include "stack.h"
#include "log.h"

void Stack_leaf(Stack **list) { *list = 0; }

int Stack_isEmpty(Stack **list) { return *list == 0; }

int Stack_size(Stack **list) {
  return Stack_isEmpty(list) ? 0 : (*list)->stack_size;
}

void Stack_new(Stack **list, void *val, int size) {
  Stack *node = (Stack *)malloc(sizeof(Stack));
  node->val = malloc(size);
  memcpy(node->val, val, size);
  Stack_leaf(&(node->next));
  node->val_size = size;
  node->stack_size = 1;
  *list = node;
}

void Stack_push(Stack **list, void *val, int size) {
  Stack *node;
  Stack_new(&node, val, size);
  if (!Stack_isEmpty(list)) {
    node->next = *list;
    node->stack_size = node->next->stack_size + 1;
  }
  *list = node;
}

void *Stack_pop(Stack **list) {
  if (Stack_isEmpty(list)) {
    log_error("cannot pop an empty stack");
    return 0;
  }
  void *value = (*list)->val;
  *list = (*list)->next;
  return value;
}

void Stack_headValue(Stack **list, Value *value) {
  value->val = (char *)(*list)->val;
  value->size = (*list)->val_size;
}

void Stack_printString(Stack **list, int comma) {
  Stack *node = *list;

  if (!Stack_isEmpty(list)) {
    Stack_printString(&node->next, 1);
    printf("%s", (char *)node->val);
    if (comma)
      printf(", ");
  }
}