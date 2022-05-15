#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/parse.h"
#include "../nobuild.h"

DECLARE_MOCK(lexeme_t, lex_peek, lex_source_t *lex);
DECLARE_MOCK(lexeme_t, lex_collect, lex_source_t *lex);
DECLARE_MOCK(lex_source_t, lex_new, char *start);
DECLARE_MOCK(int, is_lit, token_e t);
DECLARE_MOCK(int, is_un_op, token_e t);
DECLARE_MOCK(int, is_num, token_e t);
DECLARE_MOCK(int, is_high_bin, token_e t);

void test_terminal() {
  // 7;
  char *input = "7.0";
  lex_source_t lex = lex_new(input);
  MOCK(lex_peek, (lexeme_t){.tok = Dec});
  lexeme_t mocked_lex = (lexeme_t){.tok = Dec, .span = (span_t){.ptr = input}};
  MOCK(lex_collect, mocked_lex);
  MOCK(is_num, 1);
  parser_source_t parse = parser_new();

  ast_t *val = parse_terminal(&lex, &parse);
  ASSERT(val->expr_kind == Number);
  ASSERT(val->tok1.number.raw == 7);
  parser_free(&parse);

  char *input2 = "my_symbol";

  lex_source_t lex2 = lex_new(input);
  MOCK(lex_peek, (lexeme_t){.tok = Symbol});
  lexeme_t mocked_lex2 =
      (lexeme_t){.tok = Symbol, .span = (span_t){.ptr = input}};
  MOCK(lex_collect, mocked_lex2);
  parser_source_t parse2 = parser_new();

  ast_t *val2 = parse_ident(&lex2, &parse2);
  ASSERT(val2->expr_kind == Identifier);
  ASSERT(strcmp(val2->tok1.ident, input2));
  parser_free(&parse2);
}

void test_bin_op() {
  // 29;
  char *input = "7 + 5 * 2 * 2 + 2";
  lex_source_t lex = lex_new(input);
  MOCK(lex_peek, (lexeme_t){.tok = Num});
  lexeme_t mocked_lex = (lexeme_t){.tok = Num, .span = (span_t){.ptr = input}};
  MOCK(lex_collect, mocked_lex);
  MOCK(is_num, 1);
  parser_source_t parse = parser_new();

  ast_t *val = parse_low_bin(&lex, &parse);
  ASSERT(val == NULL);

  char *buffer = parser_get(&parse);
  INFO("%s", buffer);
  parser_free(&parse);
  free(buffer);
}

int main() {
  DESCRIBE("parse");
  SHOULDF("test terminal", test_terminal);
  SHOULDF("test binary operations", test_bin_op);
  RETURN();
}
