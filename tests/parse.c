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
  lex_source_t lex = lex_new("7");
  parser_source_t parse = parser_new();

  ast_t *val = parse_terminal(&lex, &parse);
  parser_print(&parse);
  ASSERT(val->expr_kind == Number);
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
