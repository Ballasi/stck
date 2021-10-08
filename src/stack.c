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
  void *next = (*list)->next;
  free(*list);
  *list = next;
  return value;
}

int Stack_get(Stack **list, Value *value, unsigned int i) {
  if (i) {
    if (Stack_isEmpty(list))
      return -1;
    return Stack_get(&(*list)->next, value, i - 1);
  }
  value->val = (char *)(*list)->val;
  value->size = (*list)->val_size;
  return 0;
}

void Stack_printString(Stack **list, int comma) {
  Stack *node = *list;

  if (!Stack_isEmpty(list)) {
    Stack_printString(&node->next, 1);
    char *ptr = (char *)node->val;
    if (ptr[0] == '\\')
      ++ptr;
    printf("%s", ptr);
    if (comma)
      printf(", ");
  }
}

int Stack_empty(Stack **list) {
  if (Stack_isEmpty(list))
    return 0;

  while (!Stack_isEmpty(list))
    free(Stack_pop(list));

  return 1;
}
