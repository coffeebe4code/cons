#pragma once
#include "byte.h"
#include "lexeme.h"

typedef enum expr_e { Expr, Number, Identifier, BinOp, UnOp } expr_e;

// Number 1 .number
// Identifier 1 .ident
// BinOp 3 .bin_left_expr .bin_op .bin_right_expr
// UnOp 1 .unary_op

typedef struct ast_t {
  expr_e expr_kind;
  union {
    byte8_t number;
    char *ident;
    struct ast_t *bin_left_expr;
    token_e unary_op;
  } tok1;
  union {
    token_e bin_op;
    struct ast_t *unary_expr;
  } tok2;
  union {
    struct ast_t *bin_right_expr;
  } tok3;
} ast_t;

#define AST_Num(val)                                                           \
  (ast_t) {                                                                    \
    .expr_kind = Number, .tok1.number = (byte8_t) { .raw = val }               \
  }

#define AST_Identifer(val)                                                     \
  (ast_t) { .expr_kind = Identifier, .tok1.ident = val }

#define AST_BinOp(left, tok, right)                                            \
  (ast_t) {                                                                    \
    .expr_kind = BinOp, .tok1.bin_left_expr = left, .tok2.bin_op = tok,        \
    .tok3.bin_right_expr = right                                               \
  }
