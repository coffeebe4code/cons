#include "../../include/gen.h"
#include "../../include/ir.h"
#include "../../include/list.h"
#include "../../include/pros.h"
#include "string.h"
#include <stdint.h>

LIST_USE(block_t, blocks, 1);
LIST_USE(instr_t, instrs, 1);

void ir_exit() {
  puts("[ERROR] | failure to allocate enough memory");
  puts("          in ir generation");
  exit(1);
}

instr_t make_instr(op_e op, byte_t dst, byte_t srcl, byte_t srcr) {
  instr_t val =
      (instr_t){.op = op, .dst = dst, .pt1.lft = srcl, .pt2.rgt = srcr};
  return val;
}

void insert_instr(ir_source_t *ir, instr_t instr) {
  instrs_add(&ir->blocks.data[ir->block_id].instructions, instr);
}

void new_block(ir_source_t *ir) {
  block_t new = (block_t){.preds = NULL,
                          .succs = NULL,
                          .label_id = ++ir->block_id,
                          .instructions = instrs_new(),
                          .kind = PlainBlock};
  if (blocks_add(&ir->blocks, new)) {
    ir_exit();
  }
}

instr_t make_data_instr(op_e op, byte_t dst, byte8_t data, size_t size) {
  instr_t val = (instr_t){
      .op = op, .dst = dst, .pt1.raw_data = data, .pt2.raw_size = size};
  return val;
}

void make_data_gen_instr(gen_source_t *gen, op_e op, byte_t dst, byte8_t data) {
  uintptr_t addr = (uintptr_t)gen->current_pos;
  if (addr % 8 != 0) {
    gen_add32(gen, make_gen_instr(op, dst, 0, 0));
  } else {
    puts("aligned already, adding noop");
    gen_add32(gen, make_gen_instr(op, dst, 1, 0));
    gen_add32(gen, make_gen_instr(NoOp, 0, 0, 0));
  }
  gen_add64(gen, data);
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
      .blocks = blocks_new(), .block_id = -1, .reg_id = 0, .gen = gen_new()};
  if (val.blocks.data == NULL) {
    ir_exit();
  }
  new_block(&val);
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
    case Mul: {
      result = ir_mulf64(ir, left, right);
      break;
    }
    case Div: {
      result = ir_divf64(ir, left, right);
      break;
    }
    case Sub: {
      result = ir_subf64(ir, left, right);
      break;
    }
    case Mod: {
      result = ir_modf64(ir, left, right);
      break;
    }
    default: {
      puts("binop not supported, this is a bug with cons");
      exit(1);
      break;
    }
    }
  default:
    break;
  }
  }
  return result;
}

void ir_flush_gen(ir_source_t *ir) {
  for (size_t i = 0; i < ir->blocks.len; i++) {
    for (size_t j = 0; j < ir->blocks.data[i].instructions.len; j++) {
      instr_t local_instr = ir->blocks.data[i].instructions.data[j];
      switch (local_instr.op) {
      case f64Const: {
        make_data_gen_instr(&ir->gen, local_instr.op, local_instr.dst,
                            local_instr.pt1.raw_data);
        break;
      }
      case Ret: {
        byte4_t val = make_gen_instr(Ret, local_instr.dst, 0, 0);
        gen_add32(&ir->gen, val);
        break;
      }
      default: {
        byte4_t val = make_gen_instr(local_instr.op, local_instr.dst,
                                     local_instr.pt1.lft, local_instr.pt2.rgt);
        gen_add32(&ir->gen, val);
        break;
      }
      }
    }
  }
}

void ir_begin(ir_source_t *ir, ast_t *main) {
  ir->main_exit = ir_recurse(ir, main);
}

size_t ir_constf64(ir_source_t *source, byte8_t data) {
  instr_t instr =
      make_data_instr(f64Const, source->reg_id++, data, sizeof(double));
  insert_instr(source, instr);
  return instr.dst;
}

size_t ir_addf64(ir_source_t *source, size_t left, size_t right) {
  instr_t instr = make_instr(f64Add, source->reg_id++, left, right);
  insert_instr(source, instr);
  return instr.dst;
}

size_t ir_ret(ir_source_t *source, size_t val) {
  instr_t instr = make_instr(Ret, val, 0, 0);
  insert_instr(source, instr);
  return instr.dst;
}

size_t ir_mulf64(ir_source_t *source, size_t left, size_t right) {
  instr_t instr = make_instr(f64Mul, source->reg_id++, left, right);
  insert_instr(source, instr);
  return instr.dst;
}

size_t ir_divf64(ir_source_t *source, size_t left, size_t right) {
  instr_t instr = make_instr(f64Div, source->reg_id++, left, right);
  insert_instr(source, instr);
  return instr.dst;
}

size_t ir_subf64(ir_source_t *source, size_t left, size_t right) {
  instr_t instr = make_instr(f64Sub, source->reg_id++, left, right);
  insert_instr(source, instr);
  return instr.dst;
}

size_t ir_modf64(ir_source_t *source, size_t left, size_t right) {
  instr_t instr = make_instr(f64Mod, source->reg_id++, left, right);
  insert_instr(source, instr);
  return instr.dst;
}

void ir_free(ir_source_t *source) {
  for (size_t i = 0; i < source->blocks.len; i++) {
    instrs_free(&source->blocks.data[i].instructions);
  }
  gen_free(&source->gen);
  blocks_free(&source->blocks);
}
