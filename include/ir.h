#pragma once
#include "ast.h"
#include "byte.h"
#include "gen.h"
#include "list.h"
#include "pros.h"
#include "stdint.h"
#include "stdio.h"

typedef enum { BPlain, BIf, BRet, BFirst } block_e;

LIST_MAKE(size_t, vars, 100);
LIST_MAKE(ir_t, irs, 100);
LIST_MAKE(byte8_t, byte8s, 100);

typedef struct {
  size_t label;
  block_e kind;
  vars_t *vars_idx;
} block_t;

typedef struct {
  pro_op_e op;
} ir_t;

typedef struct {
  byte8s_t constants;
  byte8s_t variables;
  irs_t irs;
} ir_source_t;

ir_source_t ir_new();
void ir_begin(ir_source_t *source, ast_t *main);
void ir_add(ir_source_t *source, ast_t *next);
void ir_clean(ir_source_t *source);
void ir_free(ir_source_t *source);

void ir_const64(ir_source_t *source, byte8_t left);
void ir_add64(ir_source_t *source, byte8_t left, byte8_t right);
void ir_mul64(ir_source_t *source, byte8_t left, byte8_t right);
void ir_div64(ir_source_t *source, byte8_t left, byte8_t right);
void ir_sub64(ir_source_t *source, byte8_t left, byte8_t right);
