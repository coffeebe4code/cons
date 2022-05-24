#pragma once
#include "stdint.h"

// typedefs
typedef union imm8 {
  uint8_t u;
  int8_t i;
} imm8;

typedef union imm16 {
  uint16_t u;
  int16_t i;
} imm16;

typedef union imm32 {
  uint32_t u;
  int32_t i;
} imm32;

typedef union imm64 {
  uint64_t u;
  int64_t i;
} imm64;

typedef struct instr_t {

} instr_t;

// forwards

// macros
#define COMMA ,
#define DEF_OPCODES(arch, vals) typedef enum arch##_ops_e{vals} arch##_ops_e;
#define DEF_REGS(arch, vals) typedef enum arch##_regs_e{vals} arch##_regs_e;
