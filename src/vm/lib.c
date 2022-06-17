#include "../../include/pros.h"
#include "../../include/vm.h"

// macros
#define READ_IP() (*(vm.ip))
#define INC_IP32() (vm.ip += 4)
#define INC_IP64() (vm.ip += 8)

#define STORE(val) (*vm.sp++) = vm.regs[val.raw]
#define READ_DST() ((instr & 0x00FF0000) >> 16)
#define READ_SRCL() ((instr & 0x0000FF00) >> 8)
#define READ_SRCR() (instr & 0x000000FF)

#define USE(val) vm.regs[val.raw]

#define LOAD(val) (*(--vm.sp)) = vm.regs[val];

#define BINARY_OP(op)                                                          \
  do {                                                                         \
    dst = READ_DST();                                                          \
    srcl = READ_SRCL();                                                        \
    srcr = READ_SRCR();                                                        \
    vm.regs[dst] = vm.regs[srcl] op vm.regs[srcr];                             \
    INC_IP32();                                                                \
  } while (0);

vm_t vm_new(byte_t *start) {
  vm_t val = (vm_t){
      .instrs = start, .ip = start, .regs = {0}, .stack = {0}, .sp = NULL};
  val.sp = val.stack;
  return val;
}

instr_result_e vm_run(vm_t vm) {
  instr_result_e result = OK;
  byte4_t instr;
  byte_t dst;
  byte_t srcl;
  byte_t srcr;
  int cont = 1;
  while (cont) {
    memcpy(&instr, vm.ip, sizeof(byte4_t));
    switch ((instr & 0xFF000000) >> 24) {
    case NoOp: {
      INC_IP32();
      puts("noop");
      break;
    }
    case Ret: {
      INC_IP32();
      cont = 0;
      puts("ret");
      break;
    }
    case RetVoid: {
      INC_IP32();
      cont = 0;
      puts("retvoid");
      break;
    }
    case f64Const: {
      dst = READ_DST();
      INC_IP32();
      memcpy(&(vm.regs[dst]), &READ_IP(), sizeof(byte8_t));
      INC_IP64();
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
