// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef __CMD_CONCAT_H
#define __CMD_CONCAT_H

#include "stack.h"
#include <stdio.h>
#include <string.h>

#define CMD_CONCAT { cmd_concat_test, cmd_concat_function }

static const int cmd_concat_test(char *action, int size) { return size > 2; }

static const void cmd_concat_function(Stack **buffer) {
  Value action_value;
  Stack_get(buffer, &action_value, 0);
  char *action = (char *)action_value.val;
  int size = Stack_size(buffer);

  if (action[0] == ':' && action[1] == 0) {
    free(Stack_pop(buffer));
    char temp[--size];
    for (int i = 0; i < size - 1; ++i)
      temp[i] = '.';
    temp[size - 1] = 0;
    Stack_push(buffer, temp, size);
    Stack_get(buffer, &action_value, 0);
    action = (char *)action_value.val;
    ++size;
  }
  int i = 0;
  while (action[i++] == '.') {
    if (!action[i]) {
      if (size >= i + 2) {
        free(Stack_pop(buffer));
        --size;
        int str_len = 0;
        Stack *contents;
        Stack_leaf(&contents);
        for (int j = 0; j < i + 1; ++j) {
          Value str;
          Stack_get(buffer, &str, 0);
          char *ptr = (char *)str.val;
          int len = str.size - 1;
          if (ptr[0] == '\\') {
            ++ptr;
            --len;
          }
          str_len += len + 1;
          Stack_push(&contents, ptr, len + 1);
          free(Stack_pop(buffer));
        }
        char new[str_len];
        int ptr = 0;
        while (!Stack_isEmpty(&contents)) {
          Value str;
          Stack_get(&contents, &str, 0);
          memcpy(new + ptr, (char *)str.val, str.size - 1);
          ptr += str.size;
          new[ptr - 1] = ' ';
          free(Stack_pop(&contents));
          --size;
        }
        new[str_len - 1] = 0;
        Stack_push(buffer, new, str_len);
        ++size;
      }
      break;
    }
  }
}

#endif // __CMD_CONCAT_H
