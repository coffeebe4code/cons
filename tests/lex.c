#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/lex.h"
#include "../nobuild.h"

int main() {
  DESCRIBE("lex");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
