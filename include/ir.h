#ifndef __IR__
#define __IR__
#include "ast.h"
#include "byte.h"
#include "gen.h"
#include "list.h"
#include "pros.h"

typedef struct {
  blocks_l blocks;
  gen_source_t gen;
  size_t reg_id;
  size_t block_id;
  size_t main_exit;
} ir_source_t;

ir_source_t ir_new();
void ir_begin(ir_source_t *source, ast_t *main);
void ir_flush_gen(ir_source_t *ir);
void ir_add(ir_source_t *source, ast_t *next);
void ir_insert(block_t *block, ast_t *ast);
void ir_clean(ir_source_t *source);
void ir_free(ir_source_t *source);
byte4_t make_gen_instr(op_e op, byte_t dst, byte_t srcl, byte_t srcr);

size_t ir_constf64(ir_source_t *source, byte8_t left);
size_t ir_addf64(ir_source_t *source, size_t left, size_t right);
size_t ir_mulf64(ir_source_t *source, size_t left, size_t right);
size_t ir_divf64(ir_source_t *source, size_t left, size_t right);
size_t ir_subf64(ir_source_t *source, size_t left, size_t right);
size_t ir_modf64(ir_source_t *source, size_t left, size_t right);
size_t ir_ret(ir_source_t *source, size_t val);
#endif
