#pragma once
#include "byte.h"
#include "stdio.h"

#define VM_MAX_STACK 1024
#ifdef CONSX64
#define VM_REG_COUNT 14
#else
#define VM_REG_COUNT 12
#endif

typedef struct {
  byte8_t *instrs;
  byte8_t *ip;
  byte8_t regs[VM_REG_COUNT];
  int used_regs[VM_REG_COUNT];
  byte8_t stack[VM_MAX_STACK];
  byte8_t *sp;
  byte8_t *consts;
  byte8_t *vars;
} vm_t;

typedef struct {
  size_t line;
  const char *message;
  const char **stack_trace;
} stacktrace_t;

typedef enum { OK, RUNTIME } instr_result_e;

vm_t vm_new(byte8_t *start, byte8_t *consts, byte8_t *vars);
instr_result_e vm_run(vm_t vm);
void vm_free();
