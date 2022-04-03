#pragma once
#include "span.h"
#include "token.h"

typedef enum expr_e { SExpr, Literal, BinOp, UnOp } expr_e;

typedef struct {
  expr_e expr_kind;
} ast_t;
