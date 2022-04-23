#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/ir.h"
#include "../nobuild.h"

int main() {
  DESCRIBE("ir");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
