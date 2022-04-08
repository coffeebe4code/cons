#pragma once
#include "lex.h"

typedef enum expr_e { Expr, Literal, BinOp, UnOp } expr_e;

typedef struct ast_t {
  expr_e expr_kind;
  union {
    lexed_t *literal;
    struct ast_t *bin_left_expr;
    lexed_t *unary_op;
  } tok1;
  union {
    lexed_t *bin_op;
    struct ast_t *unary_expr;
  } tok2;
  union {
    struct ast_t *bin_right_expr;
  } tok3;

} ast_t;

typedef struct entry_points_t {
  ast_t **top_level;
  int has_main;
  int is_start;
} entry_points_t;

ast_t *ast_build_graph(lex_source_t *lexer, entry_points_t *entry_points_t);
void ast_free_all(ast_t *ast);
