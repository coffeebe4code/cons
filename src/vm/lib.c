#include "../../include/bytecode.h"
#include "../../include/vm.h"

// macros
#define READ_SP() (*vm.sp++)
#define READ_CONST() (vm.values[READ_SP()])
#define POP() *(vm.values_top -= sizeof(byte8_t))
#define PUSH(val)                                                              \
  do {                                                                         \
    *vm.values_top = val;                                                      \
    vm.values_top += sizeof(byte8_t);                                          \
  } while (0)
#define BINARY_OP(op)                                                          \
  do {                                                                         \
    byte8_t b = POP();                                                         \
    byte8_t a = POP();                                                         \
    byte8_t c = (byte8_t){.raw = (a.raw op b.raw)};                            \
    PUSH(c);                                                                   \
  } while (0)

vm_t vm_new(byte_t *start, byte8_t *values) {
  vm_t val =
      (vm_t){.binary = start, .sp = start, .values_stack = {0}, NULL, values};
  val.values_top = val.values_stack;
  return val;
}

instr_result_e vm_run(vm_t vm) {
  instr_result_e result = OK;
  byte_t instr;
  int cont = 1;
  while (cont) {
    instr = READ_SP();
    switch ((bytecode_e)instr) {
    case RET: {
      cont = 0;
      break;
    }
    case CONST: {
      PUSH(READ_CONST());
      break;
    }
    case ADD: {
      // order needs to be CONST left CONST right MUL
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
