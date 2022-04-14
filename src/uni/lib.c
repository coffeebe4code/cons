#include "../../include/uni.h"

void __attribute__((constructor)) set_uni_data() {
  int i = 1;
  uni_vals.is_little_endian = (*((char *)&(i)));
}
