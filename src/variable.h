// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef __VARIABLE_H
#define __VARIABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 128

typedef unsigned int hash;

typedef struct HashElement {
  struct HashElement *next;
  char *key;
  char *value;
} HashElement;

extern HashElement *variables[HASHSIZE];

hash Variable_hash(char *s);
HashElement *Variable_get_hl(char *key);
char *Variable_get(char *key);
HashElement *Variable_put(char *key, char *value);
void Variable_empty();

#endif // __VARIABLE_H
