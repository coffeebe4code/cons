#include "../../include/gen.h"
#include "../../include/hash.h"
#include "../../include/ir.h"
#include "../../include/list.h"
#include "../../include/pros.h"
#include "string.h"

LIST_USE(instr_t, instrs, 1);
LIST_USE(block_t, blocks, 1);
LIST_USE(size_t, succs, 1);
LIST_USE(size_t, preds, 1);
LIST_USE(var_t, vars, 1);
LIST_USE(size_t, linears, 1);

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

void insert_var(ir_source_t *ir, size_t block_id, char *var_name, size_t hash,
                size_t reg_id) {
  var_t var =
      (var_t){.raw = var_name, .hashed = hash, .linears = linears_new()};
  if (var.linears.data == NULL) {
    ir_exit();
  }
  int linear_add = linears_add(&var.linears, reg_id);
  if (vars_add(&ir->blocks.data[block_id].vars, var) || linear_add) {
    ir_exit();
  }
}

void var_version(var_t *var, size_t reg_id) {
  linears_add(&var->linears, reg_id);
}

int search_var(ir_source_t *ir, size_t hash, char *name, size_t block_id) {
  int result = -1;
  vars_l local_vars = ir->blocks.data[block_id].vars;
  for (size_t i = 0; i < local_vars.len; i++) {
    if (local_vars.data[i].hashed == hash) {
      if (strcmp(name, local_vars.data[i].raw) == 0) {
        result = i;
      }
    }
  }
  return result;
}

void new_block(ir_source_t *ir, size_t hash, char *label, size_t block_id) {
  block_t new = (block_t){.vars = vars_new(),
                          .preds = preds_new(),
                          .succs = succs_new(),
                          .label = label,
                          .block_id = block_id,
                          .hash = hash,
                          .instructions = instrs_new(),
                          .kind = PlainBlock};
  if (new.instructions.data == NULL || new.preds.data == NULL ||
      new.succs.data == NULL || new.vars.data == NULL) {
    ir_exit();
  }
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
  ir_source_t val = {.main_exit = SIZE_MAX,
                     .blocks = blocks_new(),
                     .block_id = -1,
                     .reg_id = 1,
                     .gen = gen_new()};
  if (val.blocks.data == NULL) {
    ir_exit();
  }
  new_block(&val, hash, block_name, ++val.block_id);
  val.blocks.data[val.block_id].kind = TopBlock;
  return val;
}

size_t ir_recurse(ir_source_t *ir, ast_t *recurse) {
  size_t result = 0;
  switch (recurse->expr_kind) {
  case Body: {
    for (size_t i = 0; i < recurse->tok4.expr_len; i++) {
      result = ir_recurse(ir, recurse->tok2.exprs[i]);
    }
    break;
  }
  case Expr: {
    result = ir_recurse(ir, recurse->tok1.expr);
    break;
  }
  case Assign: {
    result = ir_recurse(ir, recurse->tok3.assignment);
    insert_var(ir, ir->block_id, recurse->tok1.ident_ptr->tok1.ident,
               recurse->tok1.ident_ptr->tok2.ident_hash, result);
    break;
  }
  case Identifier: {
    int var_idx = search_var(ir, recurse->tok2.ident_hash, recurse->tok1.ident,
                             ir->block_id);
    if (var_idx != -1) {
      var_t *var = &ir->blocks.data[ir->block_id].vars.data[var_idx];
      result = var->linears.data[var->linears.len - 1];
    }
    break;
  }
  case Reassign: {
    result = ir_recurse(ir, recurse->tok3.assignment);
    int var_idx = search_var(ir, recurse->tok1.ident_ptr->tok2.ident_hash,
                             recurse->tok1.ident_ptr->tok1.ident, ir->block_id);
    if (var_idx != -1) {
      var_version(&ir->blocks.data[ir->block_id].vars.data[var_idx], result);
    }
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
  default:
    puts("expression type not supported, this is a bug with cons");
    exit(1);
    break;
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
      case RetVal: {
        byte4_t val = make_gen_instr(RetVal, local_instr.dst, 0, 0);
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
  instr_t instr = make_instr(RetVal, val, 0, 0);
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
  size_t block_hash = hash_it((const char *)block_name);
  size_t block_id = -1;
  int cont = 1;
  size_t i = ir->blocks.len - 1;
  while (cont) {
    if (hash_it((const char *)ir->blocks.data[i].label) == block_hash) {
      if (strcmp(block_name, ir->blocks.data[i].label) == 0) {
        block_id = ir->blocks.data[i].block_id;
        cont = 0;
      }
    }
    i--;
  }
  return block_id;
}

void ir_free(ir_source_t *source) {
  for (size_t i = 0; i < source->blocks.len; i++) {
    instrs_free(&source->blocks.data[i].instructions);
    succs_free(&source->blocks.data[i].succs);
    preds_free(&source->blocks.data[i].preds);
    for (size_t j = 0; j < source->blocks.data[i].vars.len; j++) {
      linears_free(&source->blocks.data[i].vars.data[j].linears);
    }
    vars_free(&source->blocks.data[i].vars);
  }
  gen_free(&source->gen);
  blocks_free(&source->blocks);
}
