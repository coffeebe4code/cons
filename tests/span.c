#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/span.h"
#include "../nobuild.h"

static const char *string = "Hello";

int main() {
  DESCRIBE("span");
  SHOULDB("create a span", {
    span_t actual = span_new(string, 5);
    ASSERT(actual.ptr == string);
    ASSERT(actual.len == 5);
  });
  RETURN();
}
