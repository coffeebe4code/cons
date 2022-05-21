#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/vm.h"
#include "../nobuild.h"

int main() {
  DESCRIBE("vm");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
