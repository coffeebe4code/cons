#include "../../include/arch.h"

int is_little_endian() {
  int i = 1;
  return (*((char *)&(i)));
}
