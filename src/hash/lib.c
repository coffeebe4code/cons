#include "../../include/hash.h"

#ifdef CONS_32
#define OFFSET_HASH_BASE 2166136261UL
#else
#define OFFSET_HASH_BASE 14695981039346656037ULL
#endif

inline size_t hash_it(const char *str) {
  size_t val = 0;
  while (*str != '\0') {
    val = (*str++ ^ val) * OFFSET_HASH_BASE;
  }
  return val;
}
