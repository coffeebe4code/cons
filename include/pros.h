#ifndef __PROS__
#define __PROS__
#include "byte.h"
#include "list.h"
#include "stdio.h"

LIST_DECL(size_t, succs);
LIST_DECL(size_t, preds);

typedef enum {
  PlainBlock,
  IfBlock,
  RetBlock,
  RetBlockVoid,
  FirstBlock
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
  Invoke,
  GenInvoke,
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

  f32Const,
  f32Mul,
  f32Sub,
  f32Div,
  f32Add,

  d64Const,
  d64Mul,
  d64Sub,
  d64Div,
  d64Add,

  d32Const,
  d32Mul,
  d32Sub,
  d32Div,
  d32Add,

  i64Const,
  i64Mul,
  i64Sub,
  i64Div,
  i64Add,
  i32Const,
  i32Mul,
  i32Sub,
  i32Div,
  i32Add,
  i16Const,
  i16Mul,
  i16Sub,
  i16Div,
  i16Add,
  i8Const,
  i8Mul,
  i8Sub,
  i8Div,
  i8Add,

  u64Const,
  u64Mul,
  u64Sub,
  u64Div,
  u64Add,
  u32Const,
  u32Mul,
  u32Sub,
  u32Div,
  u32Add,
  u16Const,
  u16Mul,
  u16Sub,
  u16Div,
  u16Add,
  u8Const,
  u8Mul,
  u8Sub,
  u8Div,
  u8Add,

} op_e;

typedef struct {
  size_t idx;
} var_t;

typedef struct {
  op_e op;
  var_t dst;
  var_t lft;
  var_t rgt;
} instr_t;

LIST_DECL(instr_t, instrs);

typedef struct {
  size_t *preds;
  size_t *succs;
  size_t label;
  block_e kind;
  instr_t *instructions;
} block_t;
#endif
