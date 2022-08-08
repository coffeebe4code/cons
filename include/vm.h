#pragma once
#include "byte.h"
#include "stdio.h"

#define VM_MAX_STACK 1024
#define VM_REG_COUNT 256

typedef struct {
  byte_t *instrs;
  byte_t *ip;
  byte8_t regs[VM_REG_COUNT];
  byte8_t *sp;
  byte8_t result;
} vm_t;

vm_t vm_new(byte_t *start);
vm_t vm_run(vm_t vm);
void vm_free();
