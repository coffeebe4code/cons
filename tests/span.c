#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/span.h"

static char *string = "Hello";

int main() {
  DESCRIBE("span");
  SHOULDB("create a span", {
    span_t actual = span_new(string, 5);
    ASSERT(actual.ptr == string);
    ASSERT(actual.len == 5);
  });
  RETURN();
}
