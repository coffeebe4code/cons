#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/gen.h"
#include "../nobuild.h"

int main() {
  DESCRIBE("gen");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
