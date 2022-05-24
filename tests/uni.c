#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/uni.h"

int main() {
  DESCRIBE("uni");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
