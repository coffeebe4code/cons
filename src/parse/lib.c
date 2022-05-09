#include "../../include/ast.h"
#include "../../include/lex.h"
#include "../../include/option.h"
#include "../../include/parse.h"
#include "string.h"

void check_size(parser_source_t *parser) {
  if (parser->cap <= parser->len) {
    parser->cap <<= 2;
    parser->asts = realloc(parser->asts, parser->cap);
    parser->lexemes = realloc(parser->lexemes, parser->cap);
  }
}

parser_source_t parser_new() {
  parser_source_t val = {.len = 0, .cap = 100, .asts = NULL, .lexemes = NULL};
  val.asts = calloc(sizeof(ast_t), 100);
  val.lexemes = calloc(sizeof(lexeme_t), 100);
  return val;
}

// option_t parse_expr(lex_source_t *lexer, parser_source_t *parser) {
//  return OPTN();
//}
//
// option_t parse_bin(lex_source_t *lexer, parser_source_t *parser) {
//  return OPTN();
//}
//
// option_t parse_lit(lex_source_t *lexer, parser_source_t *parser) {
//  return OPTN();
//}
