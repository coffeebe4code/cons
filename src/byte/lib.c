#include "../../include/byte.h"
#include <inttypes.h>

byte2_t convert_byte2_be(uint16_t b) { return (byte2_t)__builtin_bswap16(b); }
byte4_t convert_byte4_be(uint32_t b) { return (byte4_t)__builtin_bswap32(b); }
byte8_t convert_byte8_be(uint64_t b) { return (byte8_t)__builtin_bswap64(b); }
