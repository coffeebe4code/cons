#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/vm.h"

typedef enum {
  Noop = 0,
  RetVoid,
  Ret,
  Load,
  Phi,
  f64Const,
  f64Mul,
  f64Sub,
  f64Div,
  f64Add
} op_e;

typedef uint8_t byte_t;

void test_bin() {
  byte8_t test_data[9] = {
      BYTE8_T_RAW(f64Const), BYTE8_T_RAW(0),      BYTE8_T_RAW(f64Const),
      BYTE8_T_RAW(1),        BYTE8_T_RAW(f64Add), BYTE8_T_RAW(f64Const),
      BYTE8_T_RAW(2),        BYTE8_T_RAW(f64Add), BYTE8_T_RAW(Ret)};
  byte8_t nums[3] = {BYTE8_T_RAW(22), BYTE8_T_RAW(55), BYTE8_T_RAW(1)};
  vm_t vm = vm_new(test_data, nums, NULL);
  instr_result_e result = vm_run(vm);

  ASSERT(result == OK);
  ASSERT((*(vm.sp)).raw == 78);
}
int main() {
  DESCRIBE("vm");
  SHOULDF("vm bin op", test_bin);
  RETURN();
}
