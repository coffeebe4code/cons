#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/parse.h"
#include "../nobuild.h"

int main() {
  DESCRIBE("parse");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
