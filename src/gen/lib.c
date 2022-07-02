#include "../../include/gen.h"
#include "stdlib.h"
#include "string.h"

void gen_exit(void *ptr) {
  if (ptr == NULL) {
    puts("[ERROR] | failure to allocate enough memory");
    puts("          in code generation");
    exit(1);
  }
}

gen_source_t gen_new() {
  gen_source_t val = {
      .len = 0, .cap = 100, .binary = NULL, .current_pos = NULL};
  val.binary = calloc(100, sizeof(unsigned char));
  gen_exit(val.binary);
  val.current_pos = val.binary;
  return val;
}

void check_size_gen(gen_source_t *gen, uint8_t size) {
  if (gen->cap <= gen->len + size) {
    gen->cap <<= 1;
    gen->binary = realloc(gen->binary, gen->cap * sizeof(byte_t));
    gen_exit(gen->binary);
  }
  gen->len += size;
}

size_t gen_add8(gen_source_t *gen, byte_t value) {
  size_t size = sizeof(value);
  check_size_gen(gen, size);
  gen->current_pos = memcpy(gen->current_pos, &value, size);
  size_t pos = (gen->current_pos - gen->binary) / sizeof(byte_t);
  gen->current_pos += size;
  return pos;
}

size_t gen_add16(gen_source_t *gen, byte2_t value) {
  size_t size = sizeof(value);
  check_size_gen(gen, size);
  gen->current_pos = memcpy(gen->current_pos, &value, size);
  size_t pos = (gen->current_pos - gen->binary) / sizeof(byte_t);
  gen->current_pos += size;
  return pos;
}

size_t gen_add32(gen_source_t *gen, byte4_t value) {
  size_t size = sizeof(value);
  check_size_gen(gen, size);
  gen->current_pos = memcpy(gen->current_pos, &value, size);
  size_t pos = (gen->current_pos - gen->binary) / sizeof(byte_t);
  gen->current_pos += size;
  return pos;
}

byte_t *gen_getbyte(gen_source_t *gen, size_t idx) {
  return (gen->binary + (idx * sizeof(byte_t)));
}

size_t gen_add64(gen_source_t *gen, byte8_t value) {
  size_t size = sizeof(value);
  check_size_gen(gen, size);
  gen->current_pos = memcpy(gen->current_pos, &value, size);
  size_t pos = (gen->current_pos - gen->binary) / sizeof(byte_t);
  gen->current_pos += size;
  return pos;
}

void gen_print_hex(gen_source_t *gen) {
  for (size_t i = 0; i < gen->len; i++) {
    printf("%x", gen->binary[i]);
  }
  printf("\n");
}

void gen_free(gen_source_t *gen) { free(gen->binary); }
