#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/bytecode.h"
#include "../nobuild.h"

int main() {
  DESCRIBE("bytecode");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
