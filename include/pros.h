#ifndef __PROS__
#define __PROS__
#include "byte.h"
#include "list.h"
#include "stdio.h"

LIST_MAKE(size_t, succs, 1);
// LIST_MAKE(size_t, preds, 1);
typedef enum { Plain, IfBlock, RetBlock, RetBlockVoid, First } block_e;
typedef enum {
  RetVoid,
  Ret,
  Load,
  f64Const,
  f64Mul,
  f64Sub,
  f64Div,
  f64Add
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

// LIST_MAKE(instr_t, instrs, 5);

typedef struct {
  size_t *preds;
  size_t *succs;
  size_t label;
  block_e kind;
  instr_t *instructions;
} block_t;
#endif
