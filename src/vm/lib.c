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

vm_t vm_new(byte8_t *start, byte8_t *consts) {
  vm_t val = (vm_t){
      .instrs = start, .ip = start, .stack = {0}, .sp = NULL, .consts = consts};
  val.sp = val.consts;
  return val;
}

instr_result_e vm_run(vm_t vm) {
  instr_result_e result = OK;
  byte8_t instr;
  int cont = 1;
  while (cont) {
    instr = READ_IP();
    switch ((pro_op_e)instr.raw) {
    case RET: {
      cont = 0;
      break;
    }
    case CONST: {
      PUSH(READ_CONST());
      break;
    }
    case ADD: {
      BINARY_OP(+);
      break;
    }
    case MUL: {
      BINARY_OP(*);
      break;
    }
    case DIV: {
      BINARY_OP(/);
      break;
    }
    case SUB: {
      BINARY_OP(-);
      break;
    }
    }
  }
  return result;
}

void vm_free();
