#include "../../include/arch.h"

#define REX_0000 0x40
#define REX_WR0B 0x4D
#define REX_WR00 0x4C
#define REX_W00B 0x49
#define REX_W000 0x40

typedef enum regs_e {
  EAX = 0x000,
  ECX = 0x001,
  EDX = 0x010,
  EBX = 0x011,
  ESP = 0x100,
  EBP = 0x101,
  ESI = 0x110,
  EDI = 0x111,
  R8 = 0x1000,
  R9 = 0x1001,
  R10 = 0x1010,
  R11 = 0x1011,
  R12 = 0x1100,
  R13 = 0x1101,
  R14 = 0x1110,
  R15 = 0x1111,
} regs_e;

int64_t addi_x64(regs_e dest, imm64) {}
