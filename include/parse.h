#pragma once
#include "ast.h"
#include "lex.h"
#include "lexeme.h"

typedef struct {
  ast_t *top_level;
} parser_source_t;

ast_t *ast_build_graph(lex_source_t *lexer, parser_source_t *parser);
void ast_free_all(ast_t *ast);
void parse_expr();
void parse_bin();
void parse_un();
int parse_lit();
