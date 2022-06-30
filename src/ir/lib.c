#include "../../include/gen.h"
#include "../../include/ir.h"
#include "../../include/list.h"
#include "../../include/pros.h"
#include "string.h"

LIST_USE(block_t, blocks, 1);
LIST_USE(instr_t, instrs, 1);

void ir_exit() {
  puts("[ERROR] | failure to allocate enough memory");
  puts("          in ir generation");
  exit(1);
}

instr_t make_ir_instr(op_e op, byte_t dst, byte_t srcl, byte_t srcr) {
  instr_t val = (instr_t){
      .op = op, .dst = dst, .lft = srcl, .data.rgt = srcr, .sizeptr = 0};
  return val;
}

void new_block(ir_source_t *ir) {
  block_t new = (block_t){.preds = NULL,
                          .succs = NULL,
                          .label_id = ir->block_id,
                          .kind = PlainBlock};
  if (blocks_add(&ir->blocks, new)) {
    ir_exit();
  }
}

void add_ir_data(instr_t *instr, void *data, size_t size) {
  instr->data.ptr = data;
  instr->sizeptr = size;
}

byte4_t make_gen_instr(op_e op, byte_t dst, byte_t srcl, byte_t srcr) {
  byte4_t val = 0;
  val = op << 24;
  val |= dst << 16;
  val |= srcl << 8;
  val |= srcr;
  return val;
}

ir_source_t ir_new() {
  ir_source_t val = {
      .blocks = blocks_new(), .block_id = 0, .instr_id = 0, .gen = gen_new()};
  if (val.blocks.data == NULL) {
    ir_exit();
  }
  return val;
}

size_t ir_recurse(ir_source_t *ir, ast_t *recurse) {
  size_t result = 0;
  switch (recurse->expr_kind) {
  case Number: {
    result = ir_constf64(ir, recurse->tok1.number);
    break;
  }
  case BinOp: {
    size_t left = ir_recurse(ir, recurse->tok1.bin_left_expr);
    size_t right = ir_recurse(ir, recurse->tok3.bin_right_expr);
    switch (recurse->tok2.bin_op) {
    case Plus: {
      result = ir_addf64(ir, left, right);
      break;
    }
    default: {
      puts("binop not implemented");
      break;
    }
    }
  default:
    break;
  }
  }
  return result;
}

void ir_begin(ir_source_t *ir, ast_t *main) {
  ir->main_exit = ir_recurse(ir, main);
}

size_t ir_constf64(ir_source_t *source, byte8_t left) {
  size_t dst = source->instr_id++;
  byte4_t instr = make_gen_instr(f64Const, dst, 0, 0);
  byte4_t noop = make_gen_instr(NoOp, 0, 0, 0);
  gen_add32(&source->gen, instr);
  gen_add32(&source->gen, noop);
  gen_add64(&source->gen, left);
  return dst;
}

size_t ir_addf64(ir_source_t *source, size_t left, size_t right) {
  size_t dst = source->instr_id++;
  byte4_t instr = make_gen_instr(f64Add, dst, left, right);
  byte4_t ret = make_gen_instr(Ret, source->instr_id++, 0, 0);
  source->instr_id++;
  gen_add32(&source->gen, instr);
  gen_add32(&source->gen, ret);
  return dst;
}
void ir_clean(ir_source_t *source) { blocks_free(&source->blocks); }

void ir_free(ir_source_t *source) { blocks_free(&source->blocks); }
