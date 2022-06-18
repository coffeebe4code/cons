#ifndef __IR__
#define __IR__
#include "ast.h"
#include "byte.h"
#include "gen.h"
#include "list.h"
#include "pros.h"

LIST_DECL(block_t, blocks);
typedef struct {
  blocks_l blocks;
} ir_source_t;

ir_source_t ir_new();
void ir_begin(ir_source_t *source, ast_t *main);
void ir_add(ir_source_t *source, ast_t *next);
void ir_clean(ir_source_t *source);
void ir_free(ir_source_t *source);

void ir_const64(ir_source_t *source, byte8_t left);
void ir_add64(ir_source_t *source, byte8_t left, byte8_t right);
void ir_mul64(ir_source_t *source, byte8_t left, byte8_t right);
void ir_div64(ir_source_t *source, byte8_t left, byte8_t right);
void ir_sub64(ir_source_t *source, byte8_t left, byte8_t right);
#endif
