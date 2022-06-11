#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/ir.h"
#include "../include/list.h"

DECLARE_MOCK_NOARGS(gen_source_t, gen_new);
DECLARE_MOCK_VOID(gen_add64, gen_source_t *val COMMA_D byte8_t byte);
// DECLARE_MOCK_T({}, block_t);
// DECLARE_MOCK_T({}, blocks_l);
// DECLARE_MOCK_NOARGS(blocks_l, blocks_new);
// DECLARE_MOCK(int, blocks_add, blocks_l *list COMMA_D block_t val);

int main() {
  DESCRIBE("ir");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
