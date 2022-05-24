#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/byte.h"

int main() {
  DESCRIBE("byte");
  SHOULDB("convert to big endian byte series", {
    byte2_t val = BYTE2(0xFF00);
    ASSERT(val.arr[0] == 0xFF);
    ASSERT(val.arr[1] == 0x00);
    ASSERT(val.raw == 0x00FF);

    byte4_t val2 = BYTE4(0xFF001100);
    ASSERT(val2.arr[0] == 0xFF);
    ASSERT(val2.arr[1] == 0x00);
    ASSERT(val2.arr[2] == 0x11);
    ASSERT(val2.arr[3] == 0x00);
    ASSERT(val2.raw == 0x001100FF);

    byte4_t addi = BYTE4(0x4C03470A);
    ASSERT(addi.arr[0] == 0x4C);
    ASSERT(addi.arr[1] == 0x03);
    ASSERT(addi.arr[2] == 0x47);
    ASSERT(addi.arr[3] == 0x0A);
    ASSERT(addi.raw == 0x0A47034C);

    byte8_t val3 = BYTE8(0xFF00110000000000);
    ASSERT(val3.arr[0] == 0xFF);
    ASSERT(val3.arr[1] == 0x00);
    ASSERT(val3.arr[2] == 0x11);
    ASSERT(val3.arr[3] == 0x00);
    ASSERT(val3.arr[4] == 0x00);
    ASSERT(val3.arr[5] == 0x00);
    ASSERT(val3.arr[6] == 0x00);
    ASSERT(val3.arr[7] == 0x00);
    ASSERT(val3.raw == 0x0000001100FF);
  });
  RETURN();
}
