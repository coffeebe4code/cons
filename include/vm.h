#pragma once
#include "byte.h"
#include "stdio.h"

#define VM_MAX_STACK 1024
#define VM_REG_COUNT 256

typedef struct {
  byte_t *instrs;
  byte_t *ip;
  byte8_t regs[VM_REG_COUNT];
  byte8_t stack[VM_MAX_STACK];
  byte8_t *sp;
} vm_t;

typedef struct {
  size_t line;
  const char *message;
  const char **stack_trace;
} stacktrace_t;

typedef enum { OK, RUNTIME } instr_result_e;

vm_t vm_new(byte_t *start);
vm_t vm_run(vm_t vm);
void vm_free();
