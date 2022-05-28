#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/cli.h"

int main() {
  DESCRIBE("cli");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
