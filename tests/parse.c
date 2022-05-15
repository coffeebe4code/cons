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
  parser_print(&parse);
}

// void test_num() {
//  // 29;
//  lex_source_t lex = lex_new("7 + 5 * 2 * 2 + 2");
//  parser_source_t parse = parser_new();
//}
//
// void test_all() {
//  // 29;
//  lex_source_t lex = lex_new("7 + 5 * 2 * 2 + 2");
//  parser_source_t parse = parser_new();
//}

int main() {
  DESCRIBE("parse");
  SHOULDF("test bin ops", test_terminal);
  RETURN();
}
