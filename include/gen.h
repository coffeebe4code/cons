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
size_t gen_add8(gen_source_t *gen, byte_t value);
size_t gen_add16(gen_source_t *gen, byte2_t value);
size_t gen_add32(gen_source_t *gen, byte4_t value);
size_t gen_add64(gen_source_t *gen, byte8_t value);

byte_t *gen_getbyte(gen_source_t *gen, size_t idx);

void gen_print_hex(gen_source_t *gen);
void gen_free(gen_source_t *gen);
#endif
