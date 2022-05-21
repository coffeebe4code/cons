#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/vm.h"
#include "../nobuild.h"

DECLARE_MOCK_E({CONST COMMA ADD COMMA RET}, bytecode_e);
typedef uint8_t byte_t;

void test_bin() {
  byte_t test_data[6] = {CONST, 0, CONST, 1, ADD, RET};
  byte8_t nums[2] = {(byte8_t){22}, (byte8_t){55}};
  vm_t vm = vm_new(test_data, nums);
  instr_result_e result = vm_run(vm);
  ASSERT((*(vm.values_top - sizeof(byte8_t))).raw == 77);
  ASSERT(result == OK);
}
int main() {
  DESCRIBE("vm");
  SHOULDF("vm bin op", test_bin);
  RETURN();
}
