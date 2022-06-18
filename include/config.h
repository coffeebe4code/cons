#pragma once
#include "stdint.h"

/// typedefs
typedef struct config_t {
  int is_little_endian;
} config_t;

/// forwards
void __attribute__((constructor)) set_uni_data();
static config_t config_vals __attribute__((unused));
