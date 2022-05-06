#pragma once
#include "lexeme.h"

typedef enum expr_e { Expr, Literal, BinOp, UnOp } expr_e;

typedef struct ast_t {
  expr_e expr_kind;
  union {
    lexeme_t *literal;
    struct ast_t *bin_left_expr;
    lexeme_t *unary_op;
  } tok1;
  union {
    lexeme_t *bin_op;
    struct ast_t *unary_expr;
  } tok2;
  union {
    struct ast_t *bin_right_expr;
  } tok3;

} ast_t;

#define AST_LIT(val)                                                           \
  (ast_t) { .expr_kind = Literal, .tok1.literal = val }
