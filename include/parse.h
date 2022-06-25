#pragma once
#include "ast.h"
#include "lex.h"

typedef struct {
  ast_t *asts;
  size_t cap;
  size_t len;
} parser_source_t;

parser_source_t parser_new();
void parser_free(parser_source_t *parser);
ast_t *parse_expr(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_high_bin(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_low_bin(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_terminal(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_num(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_ident(lex_source_t *lexer, parser_source_t *parser);
int parse_un(lex_source_t *lexer);
ast_t *parse_lit(lex_source_t *lexer, parser_source_t *parser);
char *parser_get(ast_t *highest);
