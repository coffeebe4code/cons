#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/list.h"

LIST_MAKE(char, cstrs, 2);

void test_list() {
  cstrs_t val = cstrs_new();
  cstrs_add(&val, 'a');
  cstrs_add(&val, 'b');
  cstrs_add(&val, 'c');
  ASSERT(val.data[0] == 'a');
  ASSERT(val.data[1] == 'b');
  ASSERT(val.data[2] == 'c');
  free(val.data);
}

int main() {
  DESCRIBE("list");
  SHOULDF("test list behavior with realloc", test_list);
  RETURN();
}
