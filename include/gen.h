#pragma once
#include "stdint.h"
#include "stdio.h"

typedef struct gen_source_t {
  size_t len;
  size_t cap;
  unsigned char *binary;
  unsigned char *current_pos;
} gen_source_t;

gen_source_t gen_new();
void gen_add8(gen_source_t *gen, uint8_t value);
void gen_add16(gen_source_t *gen, uint16_t value);
void gen_add32(gen_source_t *gen, uint32_t value);
void gen_add64(gen_source_t *gen, uint64_t value);
void gen_print_hex(gen_source_t *gen);
void gen_free(gen_source_t *gen);
