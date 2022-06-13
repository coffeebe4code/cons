#include "../../include/pros.h"
#include "../../include/vm.h"

// macros
#define READ_IP() (*(vm.ip++))

#define READ_CONST()                                                           \
  do {                                                                         \
    byte8_t temp = (vm.consts[READ_IP().raw]);                                 \
    vm.regs[READ_IP().raw] = temp;                                             \
  } while (0);

#define STORE(val) (*vm.sp++) = vm.regs[val.raw]

#define USE(val) vm.regs[val.raw]

#define LOAD(val) (*(--vm.sp)) = vm.regs[val];

#define BINARY_OP(op)                                                          \
  do {                                                                         \
    byte8_t dst = vm.regs[READ_IP().raw];                                      \
    byte8_t left = vm.regs[READ_IP().raw];                                     \
    byte8_t right = vm.regs[READ_IP().raw];                                    \
    vm.regs[dst.raw].raw = vm.regs[left.raw].raw op vm.regs[right.raw].raw;    \
  } while (0);

vm_t vm_new(byte8_t *start, byte8_t *consts, byte8_t *vars) {
  vm_t val = (vm_t){.instrs = start,
                    .ip = start,
                    .regs = {{0}},
                    .stack = {{0}},
                    .sp = NULL,
                    .used_regs = {0},
                    .consts = consts,
                    .vars = vars};
  val.sp = val.stack;
  return val;
}

instr_result_e vm_run(vm_t vm) {
  instr_result_e result = OK;
  byte8_t instr;
  int cont = 1;
  while (cont) {
    instr = READ_IP();
    switch ((op_e)instr.raw) {
    case Noop: {
      break;
    }
    case Ret: {
      cont = 0;
      STORE(USE(READ_IP()));
      break;
    }
    case RetVoid: {
      cont = 0;
      break;
    }
    case f64Const: {
      READ_CONST();
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
      cont = 0;
      break;
    }
    }
  }
  return result;
}

void vm_free();
