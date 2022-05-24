#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/ast.h"

int main() {
  DESCRIBE("ast");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
