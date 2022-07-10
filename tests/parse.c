#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/parse.h"

DECLARE_MOCK(lexeme_t, lex_peek, lex_source_t *lex);
DECLARE_MOCK(lexeme_t, lex_collect, lex_source_t *lex);
DECLARE_MOCK(lex_source_t, lex_new, char *start);
DECLARE_MOCK(int, is_lit, token_e t);
DECLARE_MOCK(int, is_un_op, token_e t);
DECLARE_MOCK(int, is_num, token_e t);
DECLARE_MOCK(int, is_high_bin, token_e t);
DECLARE_MOCK(int, is_low_bin, token_e t);
DECLARE_MOCK(int, is_comp, token_e t);
DECLARE_MOCK(int, is_reassign, token_e t);
DECLARE_MOCK(size_t, hash, const char *str);

void reset() {
  MOCK_RESET(lex_peek);
  MOCK_RESET(lex_collect);
  MOCK_RESET(lex_new);
  MOCK_RESET(is_lit);
  MOCK_RESET(is_un_op);
  MOCK_RESET(is_num);
  MOCK_RESET(is_high_bin);
  MOCK_RESET(is_low_bin);
  MOCK_RESET(is_comp);
  MOCK_RESET(is_reassign);
}

void test_terminal() {
  reset();
  char *input = "7.0";
  lex_source_t lex = lex_new(input);
  MOCK(lex_peek, (lexeme_t){.tok = Dec});
  lexeme_t mocked_lex = (lexeme_t){.tok = Dec, .span = (span_t){.ptr = input}};
  MOCK(lex_collect, mocked_lex);
  MOCK(is_num, 1);
  parser_source_t parse = parser_new();

  ast_t *val = parse_terminal(&lex, &parse);
  ASSERT(val->expr_kind == Number);
  ASSERT(val->tok1.number == 7);
  parser_free(&parse);

  char *input2 = "my_symbol";

  lex_source_t lex2 = lex_new(input);
  MOCK(lex_peek, (lexeme_t){.tok = Symbol});
  lexeme_t mocked_lex2 =
      (lexeme_t){.tok = Symbol, .span = (span_t){.ptr = input2, .len = 9}};
  MOCK(lex_collect, mocked_lex2);
  parser_source_t parse2 = parser_new();

  ast_t *val2 = parse_ident(&lex2, &parse2);
  ASSERT(val2->expr_kind == Identifier);
  ASSERT_STR_EQ(val2->tok1.ident, input2);
  free(val2->tok1.ident);
  parser_free(&parse2);
}

void test_returns() {
  reset();
  char *input = "return 7;";
  lex_source_t lex = lex_new(input);
  parser_source_t parse = parser_new();
  MOCK(lex_peek, (lexeme_t){.tok = Return});
  MOCK(lex_peek, (lexeme_t){.tok = Num});
  MOCK(lex_peek, (lexeme_t){.tok = Num});
  MOCK(lex_peek, (lexeme_t){.tok = Num});
  MOCK(lex_peek, (lexeme_t){.tok = Num});
  MOCK(lex_peek, (lexeme_t){.tok = SColon});
  lexeme_t mocked_lex =
      (lexeme_t){.tok = Return, .span = (span_t){.ptr = input}};
  MOCK(lex_collect, mocked_lex);
  mocked_lex = (lexeme_t){.tok = Num, .span = (span_t){.ptr = input + 7}};
  MOCK(lex_collect, mocked_lex);
  mocked_lex = (lexeme_t){.tok = SColon, .span = (span_t){.ptr = input + 8}};
  MOCK(lex_collect, mocked_lex);
  MOCK(is_comp, 0);
  MOCK(is_low_bin, 0);
  MOCK(is_high_bin, 0);
  MOCK(is_num, 1);
  MOCK(is_high_bin, 0);
  MOCK(is_low_bin, 0);
  MOCK(is_comp, 0);

  ast_t *val = parse_return(&lex, &parse);
  ASSERT(val->expr_kind == RetFn);
  ASSERT(val->tok1.ret->expr_kind == Number);
  ASSERT(val->tok1.ret->tok1.number == 7);
  ASSERT(val->tok4.semi == 1);

  parser_free(&parse);
}

void test_reassign() {
  reset();
  char *input = "x += 7;";
  lex_source_t lex = lex_new(input);
  parser_source_t parse = parser_new();
  MOCK(lex_peek, (lexeme_t){.tok = Symbol});
  MOCK(lex_peek, (lexeme_t){.tok = AddAs});
  MOCK(lex_peek, (lexeme_t){.tok = Num});
  MOCK(lex_peek, (lexeme_t){.tok = SColon});
  MOCK(lex_peek, (lexeme_t){.tok = SColon});
  MOCK(lex_peek, (lexeme_t){.tok = SColon});
  MOCK(lex_peek, (lexeme_t){.tok = SColon});
  lexeme_t mocked_lex =
      (lexeme_t){.tok = Symbol, .span = (span_t){.ptr = input, .len = 1}};
  MOCK(lex_collect, mocked_lex);
  mocked_lex = (lexeme_t){.tok = AddAs, .span = (span_t){.ptr = input + 2}};
  MOCK(lex_collect, mocked_lex);
  mocked_lex = (lexeme_t){.tok = Num, .span = (span_t){.ptr = input + 5}};
  MOCK(lex_collect, mocked_lex);
  mocked_lex = (lexeme_t){.tok = SColon, .span = (span_t){.ptr = input + 6}};
  MOCK(lex_collect, mocked_lex);
  MOCK(is_comp, 0);
  MOCK(is_reassign, 1);
  MOCK(is_low_bin, 0);
  MOCK(is_high_bin, 0);
  MOCK(is_num, 1);
  MOCK(is_high_bin, 0);
  MOCK(is_low_bin, 0);
  MOCK(is_comp, 0);

  ast_t *val = parse_reassign(&lex, &parse);
  ASSERT(val->expr_kind == Reassign);
  ASSERT_STR_EQ(val->tok1.ident_ptr->tok1.ident, "x");
  ASSERT(val->tok2.as_op == AddAs);
  ASSERT(val->tok3.assignment->tok1.number == 7);
  ASSERT(val->tok4.semi == 1);

  free(val->tok1.ident_ptr->tok1.ident);
  parser_free(&parse);
}

void test_bin_op() {
  reset();
  char *input = "1 + 2 * 2 + 3";
  lex_source_t lex = lex_new(input);
  parser_source_t parse = parser_new();
  MOCK(lex_peek, (lexeme_t){.tok = Num});
  MOCK(lex_peek, (lexeme_t){.tok = Plus});
  MOCK(lex_peek, (lexeme_t){.tok = Num});
  MOCK(lex_peek, (lexeme_t){.tok = Mul});
  MOCK(lex_peek, (lexeme_t){.tok = Num});
  MOCK(lex_peek, (lexeme_t){.tok = Plus});
  MOCK(lex_peek, (lexeme_t){.tok = Num});
  lexeme_t mocked_lex = (lexeme_t){.tok = Num, .span = (span_t){.ptr = input}};
  MOCK(lex_collect, mocked_lex);
  mocked_lex = (lexeme_t){.tok = Plus, .span = (span_t){.ptr = input + 2}};
  MOCK(lex_collect, mocked_lex);
  mocked_lex = (lexeme_t){.tok = Num, .span = (span_t){.ptr = input + 4}};
  MOCK(lex_collect, mocked_lex);
  mocked_lex = (lexeme_t){.tok = Mul, .span = (span_t){.ptr = input + 6}};
  MOCK(lex_collect, mocked_lex);
  mocked_lex = (lexeme_t){.tok = Num, .span = (span_t){.ptr = input + 8}};
  MOCK(lex_collect, mocked_lex);
  mocked_lex = (lexeme_t){.tok = Plus, .span = (span_t){.ptr = input + 10}};
  MOCK(lex_collect, mocked_lex);
  mocked_lex = (lexeme_t){.tok = Num, .span = (span_t){.ptr = input + 12}};
  MOCK(lex_collect, mocked_lex);
  MOCK(is_num, 1);
  MOCK(is_low_bin, 1);
  MOCK(is_num, 1);
  MOCK(is_high_bin, 0);
  MOCK(is_high_bin, 1);
  MOCK(is_high_bin, 0);
  MOCK(is_low_bin, 1);
  MOCK(is_num, 1);
  MOCK(is_num, 1);

  ast_t *val = parse_low_bin(&lex, &parse);
  ASSERT(val->expr_kind == BinOp);
  ASSERT(val->tok2.bin_op == Plus);
  ASSERT((int)val->tok3.bin_right_expr->tok1.number == 3);
  ASSERT(val->tok1.bin_left_expr->tok2.bin_op == Plus);
  ASSERT((int)val->tok1.bin_left_expr->tok1.bin_left_expr->tok1.number == 1);
  ASSERT((int)val->tok1.bin_left_expr->tok3.bin_right_expr->tok2.bin_op == Mul);
  ASSERT((int)val->tok1.bin_left_expr->tok3.bin_right_expr->tok1.bin_left_expr
             ->tok1.number == 2);
  ASSERT((int)val->tok1.bin_left_expr->tok3.bin_right_expr->tok3.bin_right_expr
             ->tok1.number == 2);

  parser_free(&parse);
}

int main() {
  DESCRIBE("parse");
  SHOULDF("test terminal", test_terminal);
  SHOULDF("test binary operations", test_bin_op);
  SHOULDF("test returns", test_returns);
  SHOULDF("test reassignment", test_reassign);
  RETURN();
}
