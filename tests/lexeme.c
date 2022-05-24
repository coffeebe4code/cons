#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/lexeme.h"

int main() {
  DESCRIBE("lexeme");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
