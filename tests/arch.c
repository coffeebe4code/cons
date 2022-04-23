#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/arch.h"
#include "../nobuild.h"

int main() {
  DESCRIBE("arch");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
