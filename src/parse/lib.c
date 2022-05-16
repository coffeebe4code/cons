#include "../../include/ast.h"
#include "../../include/lex.h"
#include "../../include/lexeme.h"
#include "../../include/parse.h"
#include "stdio.h"
#include "string.h"

void check_size(parser_source_t *parser) {
  if (parser->cap <= parser->len) {
    parser->cap <<= 2;
    void *ret = realloc(parser->asts, parser->cap);
    if (ret == NULL) {
    }
  }
}

parser_source_t parser_new() {
  parser_source_t val = {.len = 0, .cap = 100, .asts = NULL};
  val.asts = calloc(sizeof(ast_t), 100);
  return val;
}

ast_t *parser_add(parser_source_t *parser, ast_t ast) {
  check_size(parser);
  void *ret = memcpy(&parser->asts[parser->len++], &ast, sizeof(ast_t));
  return ret;
}

ast_t *parse_ident(lex_source_t *lexer, parser_source_t *parser) {
  if (lex_peek(lexer).tok == Symbol) {
    lexeme_t val = lex_collect(lexer);
    char *symbol = malloc(sizeof(char) * val.span.len + 1);
    memcpy(symbol, val.span.ptr, val.span.len);
    symbol[val.span.len] = '\0';
    ast_t ast = AST_Identifer(symbol);
    return parser_add(parser, ast);
  }
  return NULL;
}

ast_t *parse_num(lex_source_t *lexer, parser_source_t *parser) {
  if (is_num(lex_peek(lexer).tok)) {
    lexeme_t val = lex_collect(lexer);
    ast_t ast = AST_Num(atof(val.span.ptr));
    return parser_add(parser, ast);
  }
  return NULL;
}

ast_t *parse_terminal(lex_source_t *lexer, parser_source_t *parser) {
  ast_t *val = NULL;
  val = parse_num(lexer, parser);
  if (val == NULL) {
    val = parse_ident(lexer, parser);
  }
  return val;
}

ast_t *parse_low_bin(lex_source_t *lexer, parser_source_t *parser) {
  ast_t *left = parse_terminal(lexer, parser);
  if (left != NULL) {
    while (is_low_bin(lex_peek(lexer).tok)) {
      token_e tok = lex_collect(lexer).tok;
      ast_t *right = parse_high_bin(lexer, parser);
      if (right == NULL) {
        return right;
      }
      ast_t combined = AST_BinOp(left, tok, right);
      return parser_add(parser, combined);
    }
  }
  return left;
}

ast_t *parse_high_bin(lex_source_t *lexer, parser_source_t *parser) {
  ast_t *left = parse_terminal(lexer, parser);
  if (left != NULL) {
    while (is_high_bin(lex_peek(lexer).tok)) {
      token_e tok = lex_collect(lexer).tok;
      ast_t *right = parse_terminal(lexer, parser);
      if (right == NULL) {
        return right;
      }
      ast_t combined = AST_BinOp(left, tok, right);
      return parser_add(parser, combined);
    }
  }
  return left;
}

void parser_free(parser_source_t *parser) { free(parser->asts); }
