#pragma once

typedef struct {
  void *data;
  int has_val;
} option_t;

#define OPTY(ptr) (option_t){.data = ptr, has_val = 1};
