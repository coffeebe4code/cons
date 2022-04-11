#pragma once
#include "stdint.h"
#include "stdio.h"

typedef struct gen_source_t {
  size_t len;
  size_t cap;
  char *binary;
  char *current_pos;
} gen_source_t;

gen_source_t gen_new();
void gen_add(gen_source_t *gen, const char *value, uint8_t size);
void gen_free(gen_source_t *gen);
