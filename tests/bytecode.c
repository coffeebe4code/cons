#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/bytecode.h"

int main() {
  DESCRIBE("bytecode");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
