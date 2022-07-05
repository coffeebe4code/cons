#pragma once
#include "byte.h"
#include "lexeme.h"

typedef enum expr_e { Expr, Reassign, Number, Identifier, BinOp, UnOp } expr_e;

// Reassign 4 .ident_ptr .as_op .assignment .semi
// Number 2 .number .type
// Identifier 2 .ident .ident_hash
// BinOp 3 .bin_left_expr .bin_op .bin_right_expr
// UnOp 1 .unary_op

typedef struct ast_t {
  expr_e expr_kind;
  union {
    byte8_t number;
    struct ast_t *ident_ptr;
    struct ast_t *symbol;
    char *ident;
    struct ast_t *bin_left_expr;
    token_e unary_op;
  } tok1;
  union {
    token_e bin_op;
    token_e as_op;
    token_e type;
    size_t ident_hash;
    struct ast_t *unary_expr;
  } tok2;
  union {
    struct ast_t *bin_right_expr;
    struct ast_t *assignment;
  } tok3;
  union {
    int semi;
  } tok4;
} ast_t;

#define AST_Num(val)                                                           \
  (ast_t) { .expr_kind = Number, .tok1.number = val }

#define AST_Identifer(val, hash)                                               \
  (ast_t) {                                                                    \
    .expr_kind = Identifier, .tok1.ident = val, .tok2.ident_hash = hash        \
  }

#define AST_Reassign(ident, op, assignment, semi)                              \
  (ast_t) {                                                                    \
    .expr_kind = Reassign, .tok1.ident_ptr = ident, .tok2.as_op = op,          \
    .tok3.assignment = assignment, .tok4.semi = semi                           \
  }

#define AST_BinOp(left, tok, right)                                            \
  (ast_t) {                                                                    \
    .expr_kind = BinOp, .tok1.bin_left_expr = left, .tok2.bin_op = tok,        \
    .tok3.bin_right_expr = right                                               \
  }
