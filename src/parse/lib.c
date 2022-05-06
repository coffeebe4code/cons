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

option_t parse_expr(lex_source_t *lexer, parser_source_t *parser) {
  if (parse_bin(peeked)) {
    check_size(parser);
    lexeme_t val = lex_collect(lexer);
    memcpy(&parser->lexemes[parser->len], &val, sizeof(lexeme_t));
    ast_t ast = AST_LIT(&parser->lexemes[parser->len]);
    memcpy(&parser->asts[parser->len], &ast, sizeof(ast_t));
    return OPTY(&parser->asts[parser->len]);
  }
  return OPTN();
}

option_t parse_bin(lex_source_t *lexer, parser_source_t *parser) {
  token_e peeked = lex_peek(lexer).tok;
  if (is_lit(peeked)) {
    check_size(parser);
    lexeme_t val = lex_collect(lexer);
    memcpy(&parser->lexemes[parser->len], &val, sizeof(lexeme_t));
    ast_t ast = AST_LIT(&parser->lexemes[parser->len]);
    memcpy(&parser->asts[parser->len], &ast, sizeof(ast_t));
    return OPTY(&parser->asts[parser->len]);
  }
  return OPTN();
}

option_t parse_lit(lex_source_t *lexer, parser_source_t *parser) {
  token_e peeked = lex_peek(lexer).tok;
  if (is_lit(peeked)) {
    check_size(parser);
    lexeme_t val = lex_collect(lexer);
    memcpy(&parser->lexemes[parser->len], &val, sizeof(lexeme_t));
    ast_t ast = AST_LIT(&parser->lexemes[parser->len]);
    memcpy(&parser->asts[parser->len], &ast, sizeof(ast_t));
    return OPTY(&parser->asts[parser->len]);
  }
  return OPTN();
}

int parse_un(lex_source_t *lexer) {
  token_e peeked = lex_peek(lexer).tok;
  if (is_un_op(peeked)) {
    return 1;
  }
  return 0;
}
