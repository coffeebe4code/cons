#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/vm.h"

typedef enum {
  NoOp = 0,
  RetVoid,
  Ret,
  Load,
  Store,
  Phi,
  BitOp,
  IfOp,
  RouteMatch,
  CharOp,
  StringOp,
  ForOp,
  LoopOp,
  AllocOp,
  FreeOp,
  Struct,
  Array,
  SwitchOp,
  BreakOp,
  Invoke,
  GenInvoke,
  Access,
  PtrOp,
  AddrOp,
  AsyncOp,
  AwaitOp,
  FfiOp,

  f64Const,
  f64Mul,
  f64Sub,
  f64Div,
  f64Add,

  f32Const,
  f32Mul,
  f32Sub,
  f32Div,
  f32Add,

  d64Const,
  d64Mul,
  d64Sub,
  d64Div,
  d64Add,

  d32Const,
  d32Mul,
  d32Sub,
  d32Div,
  d32Add,

  i64Const,
  i64Mul,
  i64Sub,
  i64Div,
  i64Add,
  i32Const,
  i32Mul,
  i32Sub,
  i32Div,
  i32Add,
  i16Const,
  i16Mul,
  i16Sub,
  i16Div,
  i16Add,
  i8Const,
  i8Mul,
  i8Sub,
  i8Div,
  i8Add,

  u64Const,
  u64Mul,
  u64Sub,
  u64Div,
  u64Add,
  u32Const,
  u32Mul,
  u32Sub,
  u32Div,
  u32Add,
  u16Const,
  u16Mul,
  u16Sub,
  u16Div,
  u16Add,
  u8Const,
  u8Mul,
  u8Sub,
  u8Div,
  u8Add,
} op_e;

typedef uint8_t byte_t;

#define helper_exp2() BYTE_T_RAW(0), BYTE_T_RAW(0)
#define helper_exp3() BYTE_T_RAW(0), BYTE_T_RAW(0), BYTE_T_RAW(0)
#define helper_exp4() BYTE_T_RAW(0), BYTE_T_RAW(0), BYTE_T_RAW(0), BYTE_T_RAW(0)

void test_bin() {
  // clang-format off
  byte_t test_data[29] = {
      BYTE_T_RAW(f64Const), BYTE_T_RAW(0), helper_exp2(),
      helper_exp4(),
      helper_exp3(), BYTE_T_RAW(77),
      BYTE_T_RAW(f64Const), BYTE_T_RAW(1), helper_exp2(),
      helper_exp4(),
      helper_exp3(), BYTE_T_RAW(77),
      BYTE_T_RAW(f64Add), BYTE_T_RAW(1), BYTE_T_RAW(1), BYTE_T_RAW(0), BYTE_T_RAW(RetVoid)};
  // clang-format on

  vm_t vm = vm_new(test_data);
  instr_result_e result = vm_run(vm);

  ASSERT(result == OK);
  ASSERT(((vm.regs[1])).raw == 154);
}
int main() {
  DESCRIBE("vm");
  SHOULDF("vm bin op", test_bin);
  RETURN();
}
