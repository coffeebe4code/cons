#pragma once
#include "byte.h"
#include "lexeme.h"

typedef enum expr_e {
  Body,
  Argument,
  Arguments,
  TypeDecl,
  Property,
  Properties,
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

// Body       4 .args .exprs .args_len .expr_len
// Argument   2 .ident_ptr .sig
// Arguments  2 .arguments .args_len
// TypeDecl   4 .is_pub .ident_ptr .sig .properties_ptr
// Properties 2 .properties .props_len
// Property   4 .is_pub .sig .prop_ptr .semi_opt
// Expr       1 .expr
// RetFn      2 .ret _blank _blank .semi_opt
// Assign     5 .ident_ptr .sig .assignment .semi_opt .mut
// Reassign   4 .ident_ptr .as_op .assignment .semi_opt
// Number     2 .number .type
// Identifier 2 .ident .ident_hash
// BinOp      3 .bin_left_expr .bin_op .bin_right_expr
// UnOp       2 .unary_op .unary_expr
// Single     1 .single_op

typedef struct ast_t {
  expr_e expr_kind;
  union {
    byte8_t number;
    int is_pub;
    struct ast_t *ident_ptr;
    struct ast_t *args;
    struct ast_t *symbol;
    char *ident;
    struct ast_t *bin_left_expr;
    struct ast_t *expr;
    struct ast_t *ret;
    token_e unary_op;
    token_e single_op;
    struct ast_t **properties;
    struct ast_t **arguments;
  } tok1;
  union {
    token_e bin_op;
    token_e as_op;
    token_e type;
    struct ast_t **exprs;
    size_t ident_hash;
    struct ast_t *unary_expr;
    struct ast_t *prop_ptr;
    size_t props_len;
    struct ast_t *ident_ptr;
    struct ast_t *sig;
    size_t args_len;
  } tok2;
  union {
    size_t args_len;
    struct ast_t *bin_right_expr;
    struct ast_t *assignment;
    struct ast_t *sig;
  } tok3;
  union {
    size_t expr_len;
    token_e semi_opt;
    struct ast_t *properties_ptr;
  } tok4;
  union {
    token_e mut;
  } tok5;
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

#define AST_Reassign(ident, op, asgn, semi)                                    \
  (ast_t) {                                                                    \
    .expr_kind = Reassign, .tok1.ident_ptr = ident, .tok2.as_op = op,          \
    .tok3.assignment = asgn, .tok4.semi_opt = semi                             \
  }

#define AST_Return(value, semi)                                                \
  (ast_t) { .expr_kind = RetFn, .tok1.ret = value, .tok4.semi_opt = semi }

#define AST_Expr(value)                                                        \
  (ast_t) { .expr_kind = Expr, .tok1.expr = value }

#define AST_Property(pub, ptr, sign, semi)                                     \
  (ast_t) {                                                                    \
    .expr_kind = Property, .tok1.is_pub = pub, .tok2.prop_ptr = ptr,           \
    .tok3.sig = sign, .tok4.semi_opt = semi                                    \
  }

#define AST_TypeDecl(pub, ident, sign, prop_ptr)                               \
  (ast_t) {                                                                    \
    .expr_kind = TypeDecl, .tok1.is_pub = pub, .tok2.ident_ptr = ident,        \
    .tok3.sig = sign, .tok4.properties_ptr = prop_ptr                          \
  }

#define AST_Body(arguments, expressions, arg_length, expr_length)              \
  (ast_t) {                                                                    \
    .expr_kind = Body, .tok1.args = arguments, .tok2.exprs = expressions,      \
    .tok3.args_len = arg_length, .tok4.expr_len = expr_length                  \
  }

#define AST_Argument(ident, sign)                                              \
  (ast_t) { .expr_kind = Argument, .tok1.ident_ptr = ident, .tok2.sig = sign }

#define AST_Properties(props, prop_length)                                     \
  (ast_t) {                                                                    \
    .expr_kind = Properties, .tok1.properties = props,                         \
    .tok2.props_len = prop_length                                              \
  }

#define AST_Arguments(args, arg_len)                                           \
  (ast_t) {                                                                    \
    .expr_kind = Arguments, .tok1.arguments = args,                            \
    .tok2.args_len = arg_length                                                \
  }

#define AST_Properties(props, prop_length)                                     \
  (ast_t) {                                                                    \
    .expr_kind = Properties, .tok1.properties = props,                         \
    .tok2.props_len = prop_length                                              \
  }

#define AST_Assign(ident, mutability, asgn, semi)                              \
  (ast_t) {                                                                    \
    .expr_kind = Assign, .tok1.ident_ptr = ident, .tok2.type = mutability,     \
    .tok3.assignment = asgn, .tok4.semi_opt = semi                             \
  }

#define AST_BinOp(left, tok, right)                                            \
  (ast_t) {                                                                    \
    .expr_kind = BinOp, .tok1.bin_left_expr = left, .tok2.bin_op = tok,        \
    .tok3.bin_right_expr = right                                               \
  }
