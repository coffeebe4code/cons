#pragma once
#include "ast.h"
#include "byte.h"
#include "pros.h"
#include "stdint.h"
#include "stdio.h"

typedef struct {
  pro_op_e op;
  size_t gen;
  byte8_t *data;
} ir_t;

typedef struct {

} ir_source_t;

ir_source_t ir_new();
void ir_begin(ir_source_t *ir, ast_t *main);
void ir_add(ir_source_t *ir, ast_t *next);
void ir_clean(ir_source_t *ir);
void ir_free(ir_source_t *ir);

ir_t *ir_iiadd64(ir_source_t *source, byte8_t left, byte8_t right);
ir_t *ir_iradd64(ir_source_t *source, byte8_t left, byte8_t right);
ir_t *ir_rradd64(ir_source_t *source, byte8_t left, byte8_t right);
