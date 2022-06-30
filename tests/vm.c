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
  CSInvoke,
  CInvoke,
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
  f64Mod,

  f32Const,
  f32Mul,
  f32Sub,
  f32Div,
  f32Add,
  f32Mod,

  d64Const,
  d64Mul,
  d64Sub,
  d64Div,
  d64Add,
  d64Mod,

  d32Const,
  d32Mul,
  d32Sub,
  d32Div,
  d32Add,
  d32Mod,

  i64Const,
  i64Mul,
  i64Sub,
  i64Div,
  i64Add,
  i64Mod,
  i32Const,
  i32Mul,
  i32Sub,
  i32Div,
  i32Add,
  i32Mod,
  i16Const,
  i16Mul,
  i16Sub,
  i16Div,
  i16Add,
  i16Mod,
  i8Const,
  i8Mul,
  i8Sub,
  i8Div,
  i8Add,
  i8Mod,

  u64Const,
  u64Mul,
  u64Sub,
  u64Div,
  u64Add,
  u64Mod,
  u32Const,
  u32Mul,
  u32Sub,
  u32Div,
  u32Add,
  u32Mod,
  u16Const,
  u16Mul,
  u16Sub,
  u16Div,
  u16Add,
  u16Mod,
  u8Const,
  u8Mul,
  u8Sub,
  u8Div,
  u8Add,
  u8Mod,

} op_e;

typedef uint8_t byte_t;

#define helper_exp2() 0, 0
#define helper_exp3() 0, 0, 0
#define helper_exp4() 0, 0, 0, 0

void test_bin() {
  // clang-format off
  byte_t test_data[40] = {
      helper_exp2(), 0, f64Const,
      helper_exp4(),
      77, helper_exp3(), helper_exp4(),
      helper_exp2(), 1, f64Const,
      helper_exp4(),
      77, helper_exp3(), helper_exp4(),
      0, 1, 1, f64Add, helper_exp3(), RetVoid};
  // clang-format on

  vm_t vm = vm_new(test_data);
  vm = vm_run(vm);

  ASSERT(vm.regs[1] == 154);
}
int main() {
  DESCRIBE("vm");
  SHOULDF("vm bin op", test_bin);
  RETURN();
}
