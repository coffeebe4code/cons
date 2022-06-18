#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/byte.h"

int main() {
  DESCRIBE("byte");
  SHOULDB("convert to big endian byte series", {
    byte2_t val = BYTE2(0xFF00);
    ASSERT(val == 0x00FF);

    byte4_t val2 = BYTE4(0xFF001100);
    ASSERT(val2 == 0x001100FF);

    byte4_t addi = BYTE4(0x4C03470A);
    ASSERT(addi == 0x0A47034C);

    byte8_t val3 = BYTE8(0xFF00110000000000);
    ASSERT(val3 == 0x0000001100FF);
  });
  RETURN();
}
