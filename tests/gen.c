#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/gen.h"
#include "../nobuild.h"
#include "stdlib.h"

int main() {
  DESCRIBE("gen");
  SHOULDB("create a new source", {
    gen_source_t val = gen_new();
    ASSERT(val.binary != NULL);
    ASSERT(val.current_pos == val.binary);
    gen_free(&val);
  });

  SHOULDB("add some binary data", {
    gen_source_t val = gen_new();
    uint8_t clc = (uint8_t)0xf8;
    uint32_t addi = (uint32_t)0x4c03470a;
    gen_add8(&val, clc);
    gen_print_hex(&val);
    ASSERT(val.len == 1);
    ASSERT(val.binary[0] == 0xf8);
    ASSERT(val.current_pos == val.binary + 1);
    gen_add32(&val, addi);
    gen_print_hex(&val);
    ASSERT(val.len == 5);
    ASSERT(val.binary[1] == 0x4c);
    ASSERT(val.binary[2] == 0x03);
    ASSERT(val.binary[3] == 0x47);
    ASSERT(val.binary[4] == 0x0a);
  });

  RETURN();
}
