#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/ir.h"

DECLARE_MOCK_NOARGS(gen_source_t, gen_new);
DECLARE_MOCK(size_t, gen_add64, gen_source_t *val COMMA_D byte8_t byte);
DECLARE_MOCK(size_t, hash, const char *str);
DECLARE_MOCK(size_t, gen_add32, gen_source_t *val COMMA_D byte4_t byte);
DECLARE_MOCK_VOID(gen_free, gen_source_t *gen);

void test_bin_op() {
  ir_source_t source = ir_new();
  ast_t left = AST_Num(27);
  ast_t right = AST_Num(99);
  ast_t asts = AST_BinOp(&left, Plus, &right);
  ir_main(&source, &asts);
  ASSERT(source.block_id == 0);
  ASSERT(source.reg_id == 4);
  ASSERT(source.blocks.data[0].instructions.len == 3);
  ASSERT(source.main_exit == 3);
  ir_free(&source);
}

void test_instrs() {}

void test_make_gen() {
  byte4_t val = make_gen_instr(f64Add, 1, 1, 1);
  ASSERT((val & 0xFF000000) >> 24 == f64Add);
  ASSERT((val & 0x00FF0000) >> 16 == 1);
  ASSERT((val & 0x0000FF00) >> 8 == 1);
  ASSERT((val & 0x000000FF) == 1);
}

int main() {
  DESCRIBE("ir");
  SHOULDF("test_bin_op", test_bin_op);
  SHOULDF("test make_gen_instr", test_make_gen);
  SHOULDF("test instrs", test_instrs);
  RETURN();
}
