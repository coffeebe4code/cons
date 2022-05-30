#pragma once

typedef enum {
  RET = 0,
  CONST,
  ADD,
  MUL,
  SUB,
  DIV,
} pro_op_e;

// need to be lowercase to not conflict with token_e
typedef enum {
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
  arr,
  addr,
  ptr,
  unary,
} pro_types_e;
