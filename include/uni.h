#pragma once
#include "stdint.h"

/// typedefs
typedef unsigned char byte_t;
typedef enum byte4_e { HI, JA, KO, LO } byte4_e;
typedef enum byte2_e { ko, lo } byte2_e;

typedef struct uni_t {
  int is_little_endian;
  int is_windows;
  int is_darwin;
  int is_linux;
} uni_t;

typedef union byte2_t {
  uint16_t raw;
  byte_t arr[sizeof(uint16_t)];
} byte2_t;

typedef union byte4_t {
  uint32_t raw;
  byte_t arr[sizeof(uint32_t)];
} byte4_t;

typedef union byte8_t {
  uint64_t raw;
  byte_t arr[sizeof(uint64_t)];
} byte8_t;

/// forwards
void __attribute__((constructor)) set_uni_data();
static uni_t uni_vals __attribute__((unused));

/// macros
#define BYTE2(val) (byte2_t){.raw = val};
#define BYTE4(val) (byte4_t){.raw = val};
#define BYTE8(val) (byte8_t){.raw = val};
