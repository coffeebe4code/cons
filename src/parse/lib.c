#include "../../include/ast.h"
#include "../../include/lex.h"
#include "../../include/lexeme.h"
#include "../../include/parse.h"
#include "string.h"

void check_size(parser_source_t *parser) {
  if (parser->cap <= parser->len) {
    parser->cap <<= 2;
    parser->asts = realloc(parser->asts, parser->cap);
  }
}

parser_source_t parser_new() {
  parser_source_t val = {.len = 0, .cap = 100, .asts = NULL};
  val.asts = calloc(sizeof(ast_t), 100);
  return val;
}

ast_t *parse_ident(lex_source_t *lexer, parser_source_t *parser) {
  if (lex_peek(lexer).tok == Symbol) {
    lexeme_t val = lex_collect(lexer);
    return OPTY(val.span.ptr);
  }
  return OPTN();
}

ast_t *parse_num(lex_source_t *lexer, parser_source_t *parser) {
  if (is_num(lex_peek(lexer).tok)) {
    lexeme_t val = lex_collect(lexer);
    return OPTY(val.span.ptr);
  }
  return OPTN();
}

ast_t *parse_terminal(lex_source_t *lexer, parser_source_t *parser) {
  option_t val = parse_num(lexer);
  if (val.has_val == 0) {
    val = parse_ident(lexer);
  }
  return val;
}

ast_t *parse_high_bin(lex_source_t *lexer, parser_source_t *parser) {
  option_t val = parse_terminal(lexer);
  if (val.has_val != 0) {
    while (is_high_bin(lex_peek(lexer).tok)) {
      // do something.
    }
  }
  return val;
}
