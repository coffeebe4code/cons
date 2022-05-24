#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/ir.h"

int main() {
  DESCRIBE("ir");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
