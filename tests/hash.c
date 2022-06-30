#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/hash.h"

void test_hash() {
  const char *val1 = "two_hundred";
  size_t response1 = hash(val1);
  const char *val2 = "two_hundred1";
  size_t response2 = hash(val2);
  ASSERT(response1 != response2);
}

int main() {
  DESCRIBE("hash");
  SHOULDF("work", test_hash);
  RETURN();
}
