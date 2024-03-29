#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/gen.h"
#include "stdlib.h"

DECLARE_MOCK(byte4_t, convert_byte4_be, uint32_t val);

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
    byte_t clc = 0xF8;
    MOCK(convert_byte4_be, 0x0A47034C);
    byte4_t addi = BYTE4(0x4C03470A);
    size_t pos0 = gen_add8(&val, clc);
    ASSERT(pos0 == 0);
    ASSERT(val.len == 1);
    ASSERT(val.binary[0] == 0xF8);
    ASSERT(val.current_pos == val.binary + 1);
    size_t pos1 = gen_add32(&val, addi);
    ASSERT(val.len == 5);
    ASSERT(val.binary[1] == 0x4C);
    ASSERT(val.binary[2] == 0x03);
    ASSERT(val.binary[3] == 0x47);
    ASSERT(val.binary[4] == 0x0A);
    ASSERT(pos1 == 1);

    byte_t *result = gen_getbyte(&val, pos1);
    ASSERT(*result == 0x4C);
    gen_free(&val);
  });

  RETURN();
}
