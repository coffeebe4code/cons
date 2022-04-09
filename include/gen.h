#pragma once
#include "stdint.h"
#include "stdio.h"

typedef char ubyte;

typedef struct gen_source_t {
  size_t len;
  char *binary;
  char *current_pos;
  uint8_t current_offset;
} gen_source_t;

gen_source_t gen_new();
void gen_add(char *value, uint8_t size);
