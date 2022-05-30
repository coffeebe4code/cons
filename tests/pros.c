#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/pros.h"

int main() {
  DESCRIBE("pros");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
