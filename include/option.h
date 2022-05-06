#pragma once

typedef struct {
  void *data;
  int has_val;
} option_t;

#define OPTY(ptr) (option_t){.data = (void *)ptr, .has_val = 1};
#define OPTN() (option_t){.data = NULL, .has_val = 0};
