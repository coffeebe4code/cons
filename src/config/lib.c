#include "../../include/config.h"

void __attribute__((constructor)) set_uni_data() {
  int i = 1;
  config_vals.is_little_endian = (*((char *)&(i)));
}
