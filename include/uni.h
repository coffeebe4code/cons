#pragma once
#include "stdint.h"

/// typedefs
typedef struct uni_t {
  int is_little_endian;
} uni_t;

/// forwards
void __attribute__((constructor)) set_uni_data();
static uni_t uni_vals __attribute__((unused));
