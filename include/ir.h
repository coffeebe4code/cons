#pragma once
#include "ast.h"
#include "byte.h"
#include "gen.h"
#include "pros.h"
#include "stdint.h"
#include "stdio.h"

typedef enum { BPlain, BIf, BRet, BFirst } block_e;

typedef struct {
  pro_op_e op;
  size_t idx;
  size_t gen;
} ir_t;

typedef struct {
  gen_source_t target_gen;
  gen_source_t constants;
  gen_source_t variables;
  ir_t *irs;
  size_t cap;
  size_t len;
} ir_source_t;

ir_source_t ir_new();
void ir_begin(ir_source_t *source, ast_t *main);
void ir_add(ir_source_t *source, ast_t *next);
void ir_clean(ir_source_t *source);
void ir_free(ir_source_t *source);

ir_t *ir_const64(ir_source_t *source, byte8_t left);
ir_t *ir_add64(ir_source_t *source, byte8_t left, byte8_t right);
ir_t *ir_mul64(ir_source_t *source, byte8_t left, byte8_t right);
ir_t *ir_div64(ir_source_t *source, byte8_t left, byte8_t right);
ir_t *ir_sub64(ir_source_t *source, byte8_t left, byte8_t right);
