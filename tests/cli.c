#define MORE_COFFEE
#define WITH_MOCKING
#include "../include/cli.h"
#include "../nobuild.h"

int main() {
  DESCRIBE("cli.h");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
