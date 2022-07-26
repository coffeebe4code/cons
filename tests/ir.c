#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/ir.h"

DECLARE_MOCK_NOARGS(gen_source_t, gen_new);
DECLARE_MOCK(size_t, gen_add64, gen_source_t *val COMMA_D byte8_t byte);
DECLARE_MOCK(size_t, hash_it, const char *str);
DECLARE_MOCK(size_t, gen_add32, gen_source_t *val COMMA_D byte4_t byte);
DECLARE_MOCK_VOID(gen_free, gen_source_t *gen);

void reset() { MOCK_RESET(hash_it); }

void test_bin_op() {
  reset();
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

void test_vars() {
  reset();
  ir_source_t source = ir_new();
  char *x = "x";
  ast_t left = AST_Num(27);
  MOCK(hash_it, 12);
  MOCK(hash_it, 12);

  ast_t ident = AST_Identifer(x, hash_it((const char *)x));
  ast_t right = AST_Num(99);
  ast_t asts = AST_BinOp(&left, Plus, &right);
  ast_t assign = AST_Assign(&ident, 1, &asts, 1);
  ir_main(&source, &assign);
  ASSERT(source.block_id == 0);
  ASSERT(source.reg_id == 4);
  ASSERT(source.blocks.data[0].instructions.len == 3);
  ASSERT(source.blocks.data[0].vars.data[0].hashed == 12);
  ASSERT(source.blocks.data[0].vars.data[0].linears.data[0] == (size_t)3);
  ASSERT(source.main_exit == 3);
  ir_free(&source);
}

void test_make_gen() {
  byte4_t val = make_gen_instr(f64Add, 1, 1, 1);
  ASSERT((val & 0xFF000000) >> 24 == f64Add);
  ASSERT((val & 0x00FF0000) >> 16 == 1);
  ASSERT((val & 0x0000FF00) >> 8 == 1);
  ASSERT((val & 0x000000FF) == 1);
}

int main() {
  DESCRIBE("ir");
  SHOULDF("test bin_op", test_bin_op);
  SHOULDF("test make_gen_instr", test_make_gen);
  SHOULDF("test assign ir_recurse", test_vars);
  RETURN();
}
