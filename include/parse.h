#pragma once
#include "ast.h"
#include "lex.h"
#include "lexeme.h"
#include "option.h"

typedef struct {
  ast_t *asts;
  lexeme_t *lexemes;
  size_t cap;
  size_t len;
} parser_source_t;

parser_source_t parser_new();
void parser_free(ast_t *ast);
option_t parse_expr(lex_source_t *lexer, parser_source_t *parser);
option_t parse_bin(lex_source_t *lexer, parser_source_t *parser);
int parse_un(lex_source_t *lexer);
option_t parse_lit(lex_source_t *lexer, parser_source_t *parser);
