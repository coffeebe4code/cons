#ifndef __BYTE_T__
#define __BYTE_T__
#include "byte.h"
#include "stdint.h"
#include "stdio.h"

typedef struct gen_source_t {
  size_t len;
  size_t cap;
  byte_t *binary;
  byte_t *current_pos;
} gen_source_t;

gen_source_t gen_new();
void gen_add8(gen_source_t *gen, byte_t value);
void gen_add16(gen_source_t *gen, byte2_t value);
void gen_add32(gen_source_t *gen, byte4_t value);
void gen_add64(gen_source_t *gen, byte8_t value);
void gen_print_hex(gen_source_t *gen);
void gen_free(gen_source_t *gen);
#endif
