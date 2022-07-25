#include "../../include/gen.h"
#include "../../include/hash.h"
#include "../../include/ir.h"
#include "../../include/list.h"
#include "../../include/pros.h"
#include "string.h"
#include <stdint.h>

LIST_USE(instr_t, instrs, 10);
LIST_USE(block_t, blocks, 10);
LIST_USE(size_t, succs, 1);
LIST_USE(size_t, preds, 1);

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

void insert_instr(ir_source_t *ir, instr_t instr, size_t block_id) {
  instrs_add(&ir->blocks.data[block_id].instructions, instr);
}

void new_block(ir_source_t *ir, size_t hash, char *label, size_t block_id) {
  block_t new = (block_t){.preds = preds_new(),
                          .succs = succs_new(),
                          .label = label,
                          .block_id = block_id,
                          .hash = hash,
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

ir_source_t ir_new(size_t hash, char *block_name) {
  ir_source_t val = {.main_exit = -1,
                     .blocks = blocks_new(),
                     .block_id = -1,
                     .reg_id = 1,
                     .gen = gen_new()};
  if (val.blocks.data == NULL) {
    ir_exit();
  }
  new_block(&val, hash, block_name, ++val.block_id);
  return val;
}

size_t ir_recurse(ir_source_t *ir, ast_t *recurse) {
  size_t result = 0;
  switch (recurse->expr_kind) {
  case Assign: {
    result = ir_recurse(ir, recurse->tok3.assignment);

    break;
  }
  case RetFn: {
    if (recurse->tok1.ret == NULL) {
      result = ir_retvoid(ir);
      ir->blocks.data[ir->block_id].kind = RetBlockVoid;
    } else {
      size_t value = ir_recurse(ir, recurse->tok1.ret);
      result = ir_ret(ir, value);
      ir->blocks.data[ir->block_id].kind = RetBlock;
    }
    break;
  }
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
      case RetVoid: {
        byte4_t val = make_gen_instr(RetVoid, 0, 0, 0);
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

void ir_begin(ir_source_t *ir, ast_t **top, size_t top_len) {
  size_t recurse_ret = 0;
  for (size_t i = 0; i < top_len; i++) {
    recurse_ret = ir_recurse(ir, top[i]);
    if (recurse_ret == 0 &&
        ir->blocks.data[ir->block_id].kind != RetBlockVoid) {
      // post or preprocessor task to complete
      puts("post/pre processor task to complete");
      puts("not possible in cons yet");
    }
  }
}

void ir_main(ir_source_t *ir, ast_t *main) {
  ir->main_exit = ir_recurse(ir, main);
}

size_t ir_constf64(ir_source_t *source, byte8_t data) {
  instr_t instr =
      make_data_instr(f64Const, source->reg_id++, data, sizeof(double));
  insert_instr(source, instr, source->block_id);
  return instr.dst;
}

size_t ir_addf64(ir_source_t *source, size_t left, size_t right) {
  instr_t instr = make_instr(f64Add, source->reg_id++, left, right);
  insert_instr(source, instr, source->block_id);
  return instr.dst;
}

size_t ir_ret(ir_source_t *source, size_t val) {
  instr_t instr = make_instr(Ret, val, 0, 0);
  insert_instr(source, instr, source->block_id);
  return instr.dst;
}

size_t ir_retvoid(ir_source_t *source) {
  instr_t instr = make_instr(RetVoid, 0, 0, 0);
  insert_instr(source, instr, source->block_id);
  return 0;
}

size_t ir_mulf64(ir_source_t *source, size_t left, size_t right) {
  instr_t instr = make_instr(f64Mul, source->reg_id++, left, right);
  insert_instr(source, instr, source->block_id);
  return instr.dst;
}

size_t ir_divf64(ir_source_t *source, size_t left, size_t right) {
  instr_t instr = make_instr(f64Div, source->reg_id++, left, right);
  insert_instr(source, instr, source->block_id);
  return instr.dst;
}

size_t ir_subf64(ir_source_t *source, size_t left, size_t right) {
  instr_t instr = make_instr(f64Sub, source->reg_id++, left, right);
  insert_instr(source, instr, source->block_id);
  return instr.dst;
}

size_t ir_modf64(ir_source_t *source, size_t left, size_t right) {
  instr_t instr = make_instr(f64Mod, source->reg_id++, left, right);
  insert_instr(source, instr, source->block_id);
  return instr.dst;
}

size_t ir_get_block_id(ir_source_t *ir, char *block_name) {
  size_t block_hash = hash((const char *)block_name);
  size_t block_id = -1;
  for (size_t i = ir->blocks.len - 1; i >= 0; i--) {
    if (hash((const char *)ir->blocks.data[i].label) == block_hash) {
      if (strcmp(block_name, ir->blocks.data[i].label) == 0) {
        block_id = ir->blocks.data[i].block_id;
      }
    }
  }
  return block_id;
}

void ir_free(ir_source_t *source) {
  for (size_t i = 0; i < source->blocks.len; i++) {
    instrs_free(&source->blocks.data[i].instructions);
    succs_free(&source->blocks.data[i].succs);
    preds_free(&source->blocks.data[i].preds);
  }
  gen_free(&source->gen);
  blocks_free(&source->blocks);
}
