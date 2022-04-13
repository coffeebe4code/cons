#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/uni.h"
#include "../nobuild.h"

int main() {
  DESCRIBE("uni");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
