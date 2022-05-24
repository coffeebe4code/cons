#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/error.h"

int main() {
  DESCRIBE("error");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
