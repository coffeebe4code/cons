#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/interpret.h"
#include "../nobuild.h"

int main() {
  DESCRIBE("interpret");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
