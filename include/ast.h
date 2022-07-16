#pragma once
#include "byte.h"
#include "lexeme.h"

typedef enum expr_e {
  Expr,
  RetFn,
  Assign,
  Reassign,
  Number,
  Identifier,
  BinOp,
  UnOp
} expr_e;

// RetFn 2 .ret _blank _blank .semi
// Assign 4 .ident_ptr .type .assignment .semi
// Reassign 4 .ident_ptr .as_op .assignment .semi
// Number 2 .number .type
// Identifier 2 .ident .ident_hash
// BinOp 3 .bin_left_expr .bin_op .bin_right_expr
// UnOp 2 .unary_op .unary_expr

typedef struct ast_t {
  expr_e expr_kind;
  union {
    byte8_t number;
    struct ast_t *ident_ptr;
    struct ast_t *symbol;
    char *ident;
    struct ast_t *bin_left_expr;
    struct ast_t *ret;
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

#define AST_Assign(ident, mutability, asgn, has_semi)                          \
  (ast_t) {                                                                    \
    .expr_kind = Reassign, .tok1.ident_ptr = ident, .tok2.type = mutability,   \
    .tok3.assignment = asgn, .tok4.semi = has_semi                             \
  }

#define AST_BinOp(left, tok, right)                                            \
  (ast_t) {                                                                    \
    .expr_kind = BinOp, .tok1.bin_left_expr = left, .tok2.bin_op = tok,        \
    .tok3.bin_right_expr = right                                               \
  }
