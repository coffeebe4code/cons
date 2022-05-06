#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/option.h"
#include "../nobuild.h"

int main() {
  DESCRIBE("option");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
