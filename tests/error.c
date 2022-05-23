#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/error.h"

void test_errors() {
  error_t err = {0};
  ERROR(&err, "thing");
  ERROR_PTR(&err, "additional message", 5, "here x <=");
  ERROR_PTR(&err, "additional message", 0, "x <= here");
  ASSERT(1 == 1);
}

int main() {
  DESCRIBE("error");
  SHOULDF("run some error printing", test_errors);
  RETURN();
}
