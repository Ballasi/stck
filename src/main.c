// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "stack.h"
#include "utils.h"
#include "variable.h"

char *stopWords = " \n";
int inComment = 0;
int isInQuote = 0;

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
  if (!isInQuote && action[0] == '"') {
    if ((action_value.size > 2 && action[action_value.size - 2] != '"') ||
        action_value.size == 2)
      isInQuote = 1;
    else {
      int new_size = action_value.size - 2;
      char new_keyword[new_size];
      memcpy(new_keyword, action + 1, new_size - 1);
      new_keyword[new_size - 1] = 0;
      free(Stack_pop(buffer));
      Stack_push(buffer, new_keyword, new_size);
    }
  } else if (isInQuote) {
    ++isInQuote;
    if (action_value.size > 1 && action[action_value.size - 2] == '"') {
      Stack *contents;
      Stack_leaf(&contents);
      int str_len = 0;
      while (isInQuote--) {
        Value str;
        Stack_get(buffer, &str, 0);
        char *ptr = (char *)str.val;
        int len = str.size - 1;

        // removing first "
        if (isInQuote == 0) {
          ++ptr;
          --len;
        }
        // removing last "
        if (str_len == 0)
          --len;

        str_len += len + 1;
        Stack_push(&contents, ptr, len + 1);
        free(Stack_pop(buffer));
      }
      ++isInQuote;
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
  } else if (!strcmp(action, "\\n")) {
    printf("\n");
    free(Stack_pop(buffer));
  } else if (action[0] == '\\') {
    // escaping \ commands
  } else if (action[0] == '~') {
    char *value;
    if ((value = Variable_get(action + 1))) {
      free(Stack_pop(buffer));
      Stack_push(buffer, value, strlen(value) + 1);
    }
  } else if (size >= 3 && action[0] == '=' && action[1] == 0) {
    free(Stack_pop(buffer));
    char *name = (char *)Stack_pop(buffer);
    char *value = (char *)Stack_pop(buffer);
    Variable_put(name, value);
  } else if (size >= 2 && !strcmp(action, "print")) {
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

void processToken(Stack **buffer, char *str, int len) {
  if (!strcmp(str, "/*"))
    inComment = 1;
  if (inComment && !strcmp(str, "*/"))
    inComment = 0;
  else if (!inComment) {
    Stack_push(buffer, str, len + 1);
    processBuffer(buffer);
  }
}

int stck_showprompt() {
  Stack *buffer;
  Stack_leaf(&buffer);
  char buf[BUFFERSIZE];

  printf("stck v%s\n", VERSION);
  printf(" > ");

  while (fgets(buf, BUFFERSIZE, stdin)) {
    char *token = strtok(buf, stopWords);
    while (token != NULL) {
      processToken(&buffer, token, strlen(token));
      token = strtok(NULL, stopWords);
    }

    if (inComment)
      printf(" ┈ ");
    else if (Stack_isEmpty(&buffer))
      printf(" > ");
    else
      printf(" ― ");
  }

  printf("\n");
  Variable_empty();

  if (Stack_empty(&buffer)) {
    log_warn("stack isn't empty at the end of execution");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int stck_processfile(char **argv) {
  Stack *buffer;
  Stack_leaf(&buffer);
  char buf[BUFFERSIZE];

  FILE *file;

  file = fopen(argv[1], "r");
  if (file) {
    while (fscanf(file, "%[^ \n]", buf) != EOF) {
      if (buf[0] != 0)
        processToken(&buffer, buf, strlen(buf));

      fscanf(file, "%[ \n]", buf);
    }

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

  return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
  if (argc == 1) {
    return stck_showprompt();
  } else if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")) {
    printf("stck v%s\n", VERSION);
    return EXIT_SUCCESS;
  } else {
    return stck_processfile(argv);
  }
}
