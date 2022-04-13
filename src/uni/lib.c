#include "../../include/uni.h"

void set_uni_data() {
  int i = 1;
  uni_vals.is_little_endian = (*((char *)&(i)));
}
