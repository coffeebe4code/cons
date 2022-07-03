#include "../../include/ast.h"
#include "../../include/hash.h"
#include "../../include/lex.h"
#include "../../include/lexeme.h"
#include "../../include/parse.h"
#include "inttypes.h"
#include "stdio.h"
#include "string.h"

void parse_exit(void *ptr) {
  if (ptr == NULL) {
    puts("[ERROR] | failure to allocate enough memory");
    puts("          in parser");
    exit(1);
  }
}

void check_size_parser_serial(parser_source_t *parser) {
  if (parser->cap <= parser->len) {
    parser->cap <<= 1;
    parser->ez_free = realloc(parser->asts, parser->cap * sizeof(ast_t *));
    parse_exit(parser->asts);
  }
}

void check_size_parser_free(parser_source_t *parser) {
  if (parser->free_cap <= parser->free_len) {
    parser->free_cap <<= 1;
    parser->ez_free =
        realloc(parser->ez_free, parser->free_cap * sizeof(ast_t *));
    parse_exit(parser->ez_free);
  }
}

parser_source_t parser_new() {
  parser_source_t val = {.len = 0,
                         .cap = 2,
                         .asts = NULL,
                         .ez_free = NULL,
                         .free_cap = 2,
                         .free_len = 0};
  val.asts = calloc(2, sizeof(ast_t *));
  val.ez_free = calloc(2, sizeof(ast_t *));
  parse_exit(val.asts);
  return val;
}

ast_t *parser_add_loose(parser_source_t *parser, ast_t ast) {
  check_size_parser_free(parser);
  ast_t *new = malloc(sizeof(ast_t));
  memcpy(new, &ast, sizeof(ast_t));
  memcpy(&parser->ez_free[parser->free_len++], &new, sizeof(ast_t *));
  return new;
}

ast_t *parser_add_serial(parser_source_t *parser, ast_t ast) {
  check_size_parser_free(parser);
  check_size_parser_serial(parser);
  ast_t *new = malloc(sizeof(ast_t));
  memcpy(&parser->asts[parser->len++], &ast, sizeof(ast_t));
  memcpy(&parser->ez_free[parser->free_len++], &new, sizeof(ast_t *));
  return new;
}

ast_t *parse_ident(lex_source_t *lexer, parser_source_t *parser) {
  if (lex_peek(lexer).tok == Symbol) {
    lexeme_t val = lex_collect(lexer);
    char *symbol = malloc(sizeof(char) * val.span.len + 1);
    memcpy(symbol, val.span.ptr, val.span.len);
    symbol[val.span.len] = '\0';
    size_t hashval = hash(symbol);
    ast_t ast = AST_Identifer(symbol, hashval);
    return parser_add_loose(parser, ast);
  }
  return NULL;
}

ast_t *parse_num(lex_source_t *lexer, parser_source_t *parser) {
  if (is_num(lex_peek(lexer).tok)) {
    lexeme_t val = lex_collect(lexer);
    ast_t ast = AST_Num(atof(val.span.ptr));
    return parser_add_loose(parser, ast);
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
      return parser_add_loose(parser, combined);
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
      return parser_add_loose(parser, combined);
    }
  }
  return left;
}

void parser_free(parser_source_t *parser) {
  for (size_t i = 0; i < parser->free_len; i++) {
    free(parser->ez_free[i]);
  }
  free(parser->asts);
  free(parser->ez_free);
}
