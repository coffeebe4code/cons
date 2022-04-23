#pragma once
#include "ast.h"
#include "lex.h"
#include "lexeme.h"

typedef struct entry_points_t {
  ast_t **top_level;
  int has_main;
  int is_start;
} entry_points_t;

ast_t *ast_build_graph(lex_source_t *lexer, entry_points_t *entry_points_t);
void ast_free_all(ast_t *ast);
