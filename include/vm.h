#pragma once
#include "byte.h"
#include "gen.h"

#define VM_MAX_STACK 1024

typedef struct {
  byte_t *binary;
  byte_t *sp;
  byte8_t values_stack[VM_MAX_STACK];
  byte8_t *values_top;
  byte8_t *values;
} vm_t;

typedef struct {
  size_t line;
  const char *message;
  const char **stack;
} stacktrace_t;

typedef enum { OK, RUNTIME } instr_result_e;

vm_t vm_new(byte_t *start, byte8_t *values);
instr_result_e vm_run(vm_t vm);
void vm_free();
