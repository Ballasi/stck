// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "stack.h"
#include "utils.h"
#include "variable.h"

char *stopWords = " \n";

int isStopWord(char c) {
  char *ptr = stopWords;

  while (*ptr)
    if (*ptr++ == c)
      return 1;

  return c == 0;
}

void processBuffer(Stack **buffer) {
  Value action_value;
  Stack_get(buffer, &action_value, 0);
  char *action = (char *)action_value.val;
  int size = Stack_size(buffer);
  if (action[0] == '\\') {
    // escaping \ commands
    return;
  } else if (action[0] == '~') {
    char *value = Variable_get(action + 1);
    free(Stack_pop(buffer));
    Stack_push(buffer, value, strlen(value) + 1);
  } else if (Stack_size(buffer) >= 3 && action[0] == '=' && action[1] == 0) {
    free(Stack_pop(buffer));
    char *value = (char *) Stack_pop(buffer);
    char *name  = (char *) Stack_pop(buffer);
    Variable_put(name, value);
  } else if (Stack_size(buffer) >= 2 && !strcmp(action, "print")) {
    free(Stack_pop(buffer));
    char *value = (char *)Stack_pop(buffer);
    char *ptr = value;
    if (ptr[0] == '\\')
      ++ptr;
    printf("%s\n", ptr);
    free(value);
  } else if (!strcmp(action, "stck")) {
    free(Stack_pop(buffer));
    printf("[");
    Stack_printString(buffer, 0);
    printf("]\n");
  } else if (size > 2) {
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
    while (action[i++] == '.')
      if (!action[i]) {
        if (size >= i + 2) {
          free(Stack_pop(buffer));
          --size;
          int str_len = 0;
          Stack *contents;
          Stack_leaf(&contents);
          for (int j = 0; j < i + 1; ++j) {
            char *str = (char *)Stack_pop(buffer);
            int len = strlen(str);
            str_len += len + 1;
            char *ptr = str;
            if (ptr[0] == '\\') {
              ++ptr;
              --str_len;
            }
            Stack_push(&contents, ptr, len + 1);
            free(str);
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

void processContent(Stack **buffer, char *raw) {
  char *start = raw;
  char *ptr = raw;
  int insideBrackets = 0;
  do {
    if (*ptr == '"')
      insideBrackets = 1 - insideBrackets;
    if (isStopWord(*ptr) && ptr > start && !insideBrackets) {
      char *end = ptr - 1;
      if (*start == '"' && *end == '"') {
        start++;
        end--;
      }

      long size = end - start + 1;
      // avoid adding to stack an empty element
      if (size == 0) {
        ptr++;
        continue;
      }

      char *str = (char *)calloc(size + 1, sizeof(char));
      memcpy(str, start, size);
      Stack_push(buffer, str, size + 1);
      start = ptr + 1;
      processBuffer(buffer);
      free(str);
    }
    ptr++;
  } while (*ptr != 0);
}

int main(int argc, char **argv) {
  Stack *buffer;
  Stack_leaf(&buffer);
  char buf[BUFFERSIZE];

  if (argc == 1) {
    printf("stck v%s\n", VERSION);
    printf(" > ");
    while (fgets(buf, BUFFERSIZE, stdin)) {
      processContent(&buffer, buf);
      if (!Stack_isEmpty(&buffer))
        printf(" ― ");
      else
        printf(" > ");
    }
    printf("\n");

    Variable_empty();

    if (Stack_empty(&buffer))
      log_warn("stack isn't empty at the end of execution");
  } else {
    FILE *file;
    size_t nread;

    file = fopen(argv[1], "r");
    if (file) {
      while ((nread = fread(buf, 1, sizeof buf, file)) > 0)
        processContent(&buffer, buf);

      if (ferror(file)) {
        log_error("opening file %s", argv[1]);
        return EXIT_FAILURE;
      }

      fclose(file);
      Variable_empty();

      if (Stack_empty(&buffer))
        log_warn("stack isn't empty at the end of execution");
    } else {
      log_error("file %s does not exist", argv[1]);
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
