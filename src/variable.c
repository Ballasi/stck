// SPDX-License-Identifier: GPL-3.0-or-later

#include "variable.h"

HashElement *variables[HASHSIZE];

hash Variable_hash(char *s) {
  hash h;
  for (h = 0; *s != '\0'; s++)
    h = *s + 31 * h;
  return h % HASHSIZE;
}

char *Variable_get(char *key) {
  HashElement *hl;
  hash h = Variable_hash(key);

  for (hl = variables[h]; hl != 0; hl = hl->next)
    if (strcmp(key, hl->key) == 0)
      return hl->value;

  return 0;
}

HashElement *Variable_put(char *key, char *value) {
  HashElement *hl = 0;
  hash h;

  if (Variable_get(key) == 0) {
    hl = (HashElement *)malloc(sizeof(*hl));
    if (hl == 0 || (hl->key = key) == 0)
      return 0;
    h = Variable_hash(key);
    hl->next = variables[h];
    variables[h] = hl;
  }

  if ((hl->value = value) == 0)
    return 0;

  return hl;
}

void Variable_empty() {
  for (unsigned int i = 0; i < HASHSIZE; ++i)
    while (variables[i]) {
      HashElement *current = variables[i];
      free(current->key);
      free(current->value);
      variables[i] = current->next;
      free(current);
    }
}
