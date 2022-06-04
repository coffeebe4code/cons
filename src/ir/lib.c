#include "../../include/gen.h"
#include "../../include/ir.h"
#include "string.h"

void ir_exit(void *ptr) {
  if (ptr == NULL) {
    puts("[ERROR] | failure to allocate enough memory");
    puts("          in ir generation");
    exit(1);
  }
}

static inline void check_size(ir_source_t *ir) {
  if (ir->cap <= ir->len + 1) {
    ir->cap <<= 2;
    ir->irs = realloc(ir->irs, ir->cap * sizeof(ir_t));
    ir_exit(ir->irs);
  }
}

ir_source_t ir_new() {
  ir_source_t val = {.target_gen = gen_new(),
                     .constants = gen_new(),
                     .variables = gen_new(),
                     .irs = NULL,
                     .cap = 100,
                     .len = 0};
  val.irs = calloc(sizeof(ir_t), 100);
  ir_exit(val.irs);
  return val;
}

static inline void ir_insert(ir_source_t *source, ir_t val) {
  check_size(source);
  memcpy(&source->irs[source->len++], &val, sizeof(ir_t));
}

void ir_begin(ir_source_t *ir, ast_t *main) {
  int cont = 1;
  ast_t *curr = main;
  while (cont) {
    switch (curr->expr_kind) {
    case Number: {
      ir_t *con = ir_const64(ir, curr->tok1.number);
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

ir_t *ir_const64(ir_source_t *source, byte8_t left) {
  gen_add64(&source->constants, left);
  ir_t val = (ir_t){.op = CONST, .idx = 0, .gen = 0};
  ir_insert(source, val);
}
ir_t *ir_add64(ir_source_t *source, byte8_t left, byte8_t right);
ir_t *ir_mul64(ir_source_t *source, byte8_t left, byte8_t right);
ir_t *ir_div64(ir_source_t *source, byte8_t left, byte8_t right);
ir_t *ir_sub64(ir_source_t *source, byte8_t left, byte8_t right);
