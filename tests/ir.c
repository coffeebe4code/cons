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

void test_search() {
  reset();
  ir_source_t source = ir_new();
  char *var1 = "hello";
  char *var2 = "bye";
  char *var3 = "hello-collision";
  char *var4 = "not-found";
  insert_var(&source, 0, var1, 12ul, 0);
  insert_var(&source, 0, var2, 13ul, 1);
  insert_var(&source, 0, var3, 12ul, 2);
  int result = -1;

  result = search_var(&source, 13ul, var2, 0);
  ASSERT(result == 1);
  result = search_var(&source, 12ul, var3, 0);
  ASSERT(result == 2);
  result = search_var(&source, 12ul, var4, 0);
  ASSERT(result == -1);

  ir_free(&source);
}

void test_ir_recurse() {
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
  ast_t ident_read = AST_Identifer(x, hash_it((const char *)x));
  ast_t ret = AST_Return(&ident_read, 0);
  ast_t *serial[2] = {&assign, &ret};
  ast_t body = AST_Body(NULL, serial, 0, 2);

  ir_main(&source, &body);
  ASSERT(source.blocks.data[0].instructions.len == 4);
  ASSERT(source.blocks.data[0].instructions.data[0].dst == 1);
  ASSERT(source.blocks.data[0].instructions.data[0].op == f64Const);
  ASSERT(source.blocks.data[0].instructions.data[0].pt1.raw_data == 27);

  ASSERT(source.blocks.data[0].instructions.data[1].dst == 2);
  ASSERT(source.blocks.data[0].instructions.data[1].op == f64Const);
  ASSERT(source.blocks.data[0].instructions.data[1].pt1.raw_data == 99);

  ASSERT(source.blocks.data[0].instructions.data[2].op == f64Add);
  ASSERT(source.blocks.data[0].instructions.data[2].dst == 3);
  ASSERT(source.blocks.data[0].instructions.data[3].op == RetVal);
  ASSERT(source.blocks.data[0].instructions.data[3].dst == 3);
  // TODO:: add tests for variables, and the return statement
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
  SHOULDF("test searching vars", test_search);
  SHOULDF("test ir_recurse", test_ir_recurse);
  RETURN();
}
