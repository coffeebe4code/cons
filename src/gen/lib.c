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
  val.binary = calloc(sizeof(unsigned char), 100);
  gen_exit(val.binary);
  val.current_pos = val.binary;
  return val;
}

// BUG:: size and realloc.
void check_size(gen_source_t *gen, uint8_t size) {
  if (gen->cap <= gen->len + size) {
    gen->cap <<= 2;
    gen->binary = realloc(gen->binary, gen->cap);
    gen_exit(gen->binary);
  }
  gen->len += size;
}

// BUG:: size and realloc.
void gen_add8(gen_source_t *gen, byte_t value) {
  size_t size = sizeof(value);
  check_size(gen, size);
  gen->current_pos = memcpy(gen->current_pos, &value, size);
  gen->current_pos += size;
}

// BUG:: size and realloc.
void gen_add16(gen_source_t *gen, byte2_t value) {
  size_t size = sizeof(value);
  check_size(gen, size);
  gen->current_pos = memcpy(gen->current_pos, &value, size);
  gen->current_pos += size;
}

// BUG:: size and realloc.
void gen_add32(gen_source_t *gen, byte4_t value) {
  size_t size = sizeof(value);
  check_size(gen, size);
  gen->current_pos = memcpy(gen->current_pos, &value, size);
  gen->current_pos += size;
}

// BUG:: size and realloc.
void gen_add64(gen_source_t *gen, byte8_t value) {
  size_t size = sizeof(value);
  check_size(gen, size);
  gen->current_pos = memcpy(gen->current_pos, &value, size);
  gen->current_pos += size;
}

void gen_print_hex(gen_source_t *gen) {
  for (size_t i = 0; i < gen->len; i++) {
    printf("%x", gen->binary[i] & 0xff);
  }
  printf("\n");
}

void gen_free(gen_source_t *gen) { free(gen->binary); }
