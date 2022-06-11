#include "../../include/pros.h"
#include "../../include/vm.h"

// macros
#define READ_IP() (*vm.ip++)
#define READ_CONST() (vm.consts[READ_IP().raw])
#define POP() *(--vm.sp)
#define PUSH(val)                                                              \
  do {                                                                         \
    *vm.sp = val;                                                              \
    vm.sp++;                                                                   \
  } while (0)
#define BINARY_OP(op)                                                          \
  do {                                                                         \
    byte8_t b = POP();                                                         \
    byte8_t a = POP();                                                         \
    byte8_t c = (byte8_t){.raw = (a.raw op b.raw)};                            \
    PUSH(c);                                                                   \
  } while (0)

vm_t vm_new(byte8_t *start, byte8_t *consts, byte8_t *vars) {
  vm_t val = (vm_t){.instrs = start,
                    .ip = start,
                    .stack = {0},
                    .sp = NULL,
                    .consts = consts,
                    .vars = vars};
  val.sp = val.consts;
  return val;
}

instr_result_e vm_run(vm_t vm) {
  instr_result_e result = OK;
  byte8_t instr;
  int cont = 1;
  while (cont) {
    instr = READ_IP();
    switch ((op_e)instr.raw) {
    case Ret: {
      cont = 0;
      break;
    }
    case f64Const: {
      PUSH(READ_CONST());
      break;
    }
    case f64Add: {
      BINARY_OP(+);
      break;
    }
    case f64Mul: {
      BINARY_OP(*);
      break;
    }
    case f64Div: {
      BINARY_OP(/);
      break;
    }
    case f64Sub: {
      BINARY_OP(-);
      break;
    }
    default: {
      break;
    }
    }
  }
  return result;
}

void vm_free();
