#pragma once
#include "byte.h"
#include "lexeme.h"

typedef enum expr_e {
  Body,
  Expr,
  RetFn,
  Assign,
  Reassign,
  Number,
  Identifier,
  BinOp,
  UnOp,
  Single,
} expr_e;

// Body 4 .args .exprs .args_len .expr_len
// Expr 2 .expr .type
// RetFn 2 .ret _blank _blank .semi
// Assign 4 .ident_ptr .type .assignment .semi
// Reassign 4 .ident_ptr .as_op .assignment .semi
// Number 2 .number .type
// Identifier 2 .ident .ident_hash
// BinOp 3 .bin_left_expr .bin_op .bin_right_expr
// UnOp 2 .unary_op .unary_expr
// Single 1 .single_op

typedef struct ast_t {
  expr_e expr_kind;
  union {
    byte8_t number;
    struct ast_t *ident_ptr;
    struct ast_t *args;
    struct ast_t *symbol;
    char *ident;
    struct ast_t *bin_left_expr;
    struct ast_t *expr;
    struct ast_t *ret;
    token_e unary_op;
    token_e single_op;
  } tok1;
  union {
    token_e bin_op;
    token_e as_op;
    token_e type;
    struct ast_t *exprs;
    size_t ident_hash;
    struct ast_t *unary_expr;
  } tok2;
  union {
    size_t args_len;
    struct ast_t *bin_right_expr;
    struct ast_t *assignment;
  } tok3;
  union {
    size_t expr_len;
    int semi;
  } tok4;
} ast_t;

#define AST_Num(val)                                                           \
  (ast_t) { .expr_kind = Number, .tok1.number = val }

#define AST_Single(val)                                                        \
  (ast_t) { .expr_kind = Single, .tok1.single_op = val }

#define AST_Unary(op, val)                                                     \
  (ast_t) { .expr_kind = UnOp, .tok1.unary_op = op, .tok2.unary_expr = val }

#define AST_Identifer(val, hash)                                               \
  (ast_t) {                                                                    \
    .expr_kind = Identifier, .tok1.ident = val, .tok2.ident_hash = hash        \
  }

#define AST_Reassign(ident, op, asgn, has_semi)                                \
  (ast_t) {                                                                    \
    .expr_kind = Reassign, .tok1.ident_ptr = ident, .tok2.as_op = op,          \
    .tok3.assignment = asgn, .tok4.semi = has_semi                             \
  }

#define AST_Return(value, has_semi)                                            \
  (ast_t) { .expr_kind = RetFn, .tok1.ret = value, .tok4.semi = has_semi }

#define AST_Expr(value)                                                        \
  (ast_t) { .expr_kind = Expr, .tok1.expr = value }

#define AST_Body(arguments, expressions, arg_length, expr_length)              \
  (ast_t) {                                                                    \
    .expr_kind = Body, .tok1.args = arguments, .tok2.exprs = expressions,      \
    .tok3.args_len = arg_length, .tok4.expr_len = expr_length                  \
  }

#define AST_Assign(ident, mutability, asgn, has_semi)                          \
  (ast_t) {                                                                    \
    .expr_kind = Assign, .tok1.ident_ptr = ident, .tok2.type = mutability,     \
    .tok3.assignment = asgn, .tok4.semi = has_semi                             \
  }

#define AST_BinOp(left, tok, right)                                            \
  (ast_t) {                                                                    \
    .expr_kind = BinOp, .tok1.bin_left_expr = left, .tok2.bin_op = tok,        \
    .tok3.bin_right_expr = right                                               \
  }
