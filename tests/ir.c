#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/ir.h"
#include "../include/list.h"

DECLARE_MOCK_NOARGS(gen_source_t, gen_new);
DECLARE_MOCK(size_t, gen_add64, gen_source_t *val COMMA_D byte8_t byte);

void test_bin_op() { ASSERT(1 == 1); }

int main() {
  DESCRIBE("ir");
  SHOULDF("work", test_bin_op);
  RETURN();
}
