#pragma once
#include "stdint.h"

// typedefs
typedef uint8_t byte_t;

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

// forwards

byte2_t convert_byte2_be(uint16_t b);
byte4_t convert_byte4_be(uint32_t b);
byte8_t convert_byte8_be(uint64_t b);

/// macros
#define BYTE2(val) convert_byte2_be(val)
#define BYTE4(val) convert_byte4_be(val)
#define BYTE8(val) convert_byte8_be(val)
