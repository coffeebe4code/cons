#pragma once
#include "byte.h"
#include "list.h"
#include "stdio.h"

typedef enum {
  PlainBlock,
  IfBlock,
  RetBlock,
  RetBlockVoid,
  FirstBlock,
  TopBlock,
  LeafBlock,
} block_e;

typedef enum {
  NoOp = 0,
  RetVoid,
  Ret,
  Load,
  Store,
  Phi,
  BitOp,
  IfOp,
  RouteMatch,
  CharOp,
  StringOp,
  ForOp,
  LoopOp,
  AllocOp,
  FreeOp,
  Struct,
  Array,
  SwitchOp,
  BreakOp,
  CSInvoke,
  CInvoke,
  Access,
  PtrOp,
  AddrOp,
  AsyncOp,
  AwaitOp,
  FfiOp,

  f64Const,
  f64Mul,
  f64Sub,
  f64Div,
  f64Add,
  f64Mod,

  f32Const,
  f32Mul,
  f32Sub,
  f32Div,
  f32Add,
  f32Mod,

  d64Const,
  d64Mul,
  d64Sub,
  d64Div,
  d64Add,
  d64Mod,

  d32Const,
  d32Mul,
  d32Sub,
  d32Div,
  d32Add,
  d32Mod,

  i64Const,
  i64Mul,
  i64Sub,
  i64Div,
  i64Add,
  i64Mod,
  i32Const,
  i32Mul,
  i32Sub,
  i32Div,
  i32Add,
  i32Mod,
  i16Const,
  i16Mul,
  i16Sub,
  i16Div,
  i16Add,
  i16Mod,
  i8Const,
  i8Mul,
  i8Sub,
  i8Div,
  i8Add,
  i8Mod,

  u64Const,
  u64Mul,
  u64Sub,
  u64Div,
  u64Add,
  u64Mod,
  u32Const,
  u32Mul,
  u32Sub,
  u32Div,
  u32Add,
  u32Mod,
  u16Const,
  u16Mul,
  u16Sub,
  u16Div,
  u16Add,
  u16Mod,
  u8Const,
  u8Mul,
  u8Sub,
  u8Div,
  u8Add,
  u8Mod,

} op_e;

typedef struct {
  op_e op;
  byte_t dst;
  union {
    byte_t lft;
    byte8_t raw_data;
  } pt1;
  union {
    byte_t rgt;
    size_t raw_size;
  } pt2;
} instr_t;

LIST_DECL(instr_t, instrs);
LIST_DECL(size_t, preds);
LIST_DECL(size_t, succs);

typedef struct {
  preds_l preds;
  succs_l succs;
  size_t hash;
  size_t block_id;
  char *label;
  instrs_l instructions;
  block_e kind;
} block_t;

LIST_DECL(block_t, blocks);
