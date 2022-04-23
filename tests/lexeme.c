#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/lexeme.h"
#include "../nobuild.h"

int main() {
  DESCRIBE("lexeme");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
