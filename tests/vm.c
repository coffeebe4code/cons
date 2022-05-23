#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/vm.h"

DECLARE_MOCK_E({RET = 0 COMMA CONST COMMA ADD COMMA MUL COMMA DIV COMMA SUB},
               bytecode_e);
typedef uint8_t byte_t;

void test_bin() {
  byte8_t test_data[9] = {
      BYTE8_T_RAW(CONST), BYTE8_T_RAW(0),   BYTE8_T_RAW(CONST),
      BYTE8_T_RAW(1),     BYTE8_T_RAW(ADD), BYTE8_T_RAW(CONST),
      BYTE8_T_RAW(2),     BYTE8_T_RAW(ADD), BYTE8_T_RAW(RET)};
  byte8_t nums[3] = {BYTE8_T_RAW(22), BYTE8_T_RAW(55), BYTE8_T_RAW(1)};
  vm_t vm = vm_new(test_data, nums);
  instr_result_e result = vm_run(vm);

  ASSERT(result == OK);
  ASSERT((*(vm.sp)).raw == 78);
}
int main() {
  DESCRIBE("vm");
  SHOULDF("vm bin op", test_bin);
  RETURN();
}
