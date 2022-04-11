#include "../../include/gen.h"
#include "stdlib.h"
#include "string.h"

gen_source_t gen_new() {
  gen_source_t val = {
      .len = 0, .cap = 100, .binary = NULL, .current_pos = NULL};
  val.binary = calloc(sizeof(char *), 100);
  if (val.binary == NULL) {
    puts("error in gen_new");
  }
  val.current_pos = val.binary;
  return val;
}

void gen_add(gen_source_t *gen, const char *value, uint8_t size) {
  if (gen->cap <= gen->len + size) {
    gen->cap >>= 2;
    gen->binary = realloc(gen->binary, gen->cap);
  }
  gen->len += size;
  gen->current_pos = memcpy(gen->current_pos, value, size);
  gen->current_pos += size;
}

void gen_free(gen_source_t *gen) { free(gen->binary); }
