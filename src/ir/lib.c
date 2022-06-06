#include "../../include/gen.h"
#include "../../include/ir.h"
#include "string.h"

void ir_exit() {
  puts("[ERROR] | failure to allocate enough memory");
  puts("          in ir generation");
  exit(1);
}

ir_source_t ir_new() {
  ir_source_t val = {.constants = byte8_l_new(),
                     .variables = byte8_l_new(),
                     .irs = ir_l_new()};
  if (val.constants.data == NULL || val.variables.data == NULL ||
      val.irs.data == NULL) {
    ir_exit();
  }
  return val;
}

void ir_recurse(ir_source_t *ir, ast_t *recurse) {
  switch (recurse->expr_kind) {
  case Number: {
    ir_const64(ir, recurse->tok1.number);
    break;
  }
  case BinOp: {
    ir_recurse(ir, recurse->tok1.bin_left_expr);
    ir_recurse(ir, recurse->tok3.bin_right_expr);
    ir_recurse(ir, recurse->tok2.bin_op);

    break;
  }
  default:
    break;
  }
}

void ir_begin(ir_source_t *ir, ast_t *main) {
  int cont = 1;
  ast_t *curr = main;
  while (cont) {
    switch (curr->expr_kind) {
    case Number: {
      ir_const64(ir, curr->tok1.number);
      break;
    }
    default:
      break;
    }
  }
}
void ir_add(ir_source_t *source, ast_t *next);
void ir_clean(ir_source_t *source);
void ir_free(ir_source_t *source);

void ir_const64(ir_source_t *source, byte8_t left) {
  int insert = byte8_l_add(&source->constants, left);
  ir_t val = (ir_t){.op = CONST, .idx = 0, .gen = 0};
  insert += ir_l_add(&source->irs, val);
  if (insert) {
    ir_exit();
  }
}

void ir_add64(ir_source_t *source, byte8_t);
void ir_mul64(ir_source_t *source, byte8_t left, byte8_t right);
void ir_div64(ir_source_t *source, byte8_t left, byte8_t right);
void ir_sub64(ir_source_t *source, byte8_t left, byte8_t right);
