#pragma once
#include "stdlib.h"

typedef struct {
  char *ptr;
  size_t len;
} span_t;

span_t span_new(char *data, size_t len);
span_t span_copy(span_t *from);
