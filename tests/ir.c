#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/ir.h"
#include "../include/list.h"

DECLARE_MOCK_NOARGS(gen_source_t, gen_new);
DECLARE_MOCK(size_t, gen_add64, gen_source_t *val COMMA_D byte8_t byte);
DECLARE_MOCK(size_t, gen_add32, gen_source_t *val COMMA_D byte4_t byte);

void test_bin_op() {
  ir_source_t source = ir_new();
  ast_t left = AST_Num(27);
  ast_t right = AST_Num(99);
  ast_t asts = AST_BinOp(&left, Plus, &right);
  ir_begin(&source, &asts);
  ir_free(&source);
}

int main() {
  DESCRIBE("ir");
  SHOULDF("work", test_bin_op);
  RETURN();
}
