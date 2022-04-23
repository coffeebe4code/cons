#pragma once
#include "stdint.h"

typedef enum ir_kind_e {
  i64,
  u64,
  f64,
  d64,
  i32,
  u32,
  f32,
  d32,
  i16,
  u16,
  u8,
  i8,
  uchar,
  data,
  arr,
  addr,
  ptr,
  unary,
  branch,
  func,
  loop,
} ir_kind_e;

typedef struct ir_obj_t {
  ir_kind_e kind;
  int vs;
} ir_obj_t;

typedef struct ir_source_t {
  ir_obj_t *objs;
  int *call_graphs;
  int symbols;
} ir_source_t;

void ir_iconst(ir_source_t *source, ir_kind_e kind, uint64_t val);
void ir_iadd(ir_source_t *source, ir_kind_e kind, ir_obj_t *left,
             ir_obj_t *right);
