#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/config.h"

int main() {
  DESCRIBE("config");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
