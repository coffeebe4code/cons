#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/ir.h"

DECLARE_MOCK_NOARGS(gen_source_t, gen_new);
DECLARE_MOCK_VOID(gen_add64, gen_source_t *val COMMA_D byte8_t byte);

int main() {
  DESCRIBE("ir");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
