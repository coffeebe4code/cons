#include "../../include/span.h"

span_t span_new(char *data, size_t len) {
  span_t span = {.ptr = data, len = len};
  return span;
}

span_t span_copy(span_t *from) {
  span_t span = {.ptr = from->ptr, .len = from->len};
  return span;
}
