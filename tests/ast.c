#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/ast.h"
#include "../nobuild.h"

int main() {
  DESCRIBE("ast");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
