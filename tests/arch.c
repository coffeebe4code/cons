#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/arch.h"

int main() {
  DESCRIBE("arch");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
