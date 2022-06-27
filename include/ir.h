#ifndef __IR__
#define __IR__
#include "ast.h"
#include "byte.h"
#include "gen.h"
#include "list.h"
#include "pros.h"
#include <stddef.h>

LIST_DECL(block_t, blocks);
typedef struct {
  blocks_l blocks;
  block_t curr_block;
  gen_source_t gen;
  size_t new_idx;
  size_t main_exit;
} ir_source_t;

ir_source_t ir_new();
void ir_begin(ir_source_t *source, ast_t *main);
void ir_add(ir_source_t *source, ast_t *next);
void ir_clean(ir_source_t *source);
void ir_free(ir_source_t *source);
byte4_t make_gen_instr(op_e op, byte_t dst, byte_t srcl, byte_t srcr);

size_t ir_constf64(ir_source_t *source, byte8_t left);
size_t ir_addf64(ir_source_t *source, size_t left, size_t right);
void ir_mulf64(ir_source_t *source, byte8_t left, byte8_t right);
void ir_div64(ir_source_t *source, byte8_t left, byte8_t right);
void ir_sub64(ir_source_t *source, size_t left, size_t right);
#endif
