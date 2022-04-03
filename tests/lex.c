#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/lex.h"
#include "../nobuild.h"

char *full_string = "#include whatever";

DECLARE_MOCK(token_e, token_next, char *data COMMA int *len);
DECLARE_MOCK(span_t, span_new, char *data COMMA int len);

void test_peek() {
  lex_source_t val = lex_new(full_string);
  MOCK(token_next, 

  lex_peek(&val);
}

void test_peek_twice() {}

void test_collect() {}
int main() {
  DESCRIBE("lex");
  SHOULDF("peek", test_peek);
  SHOULDF("peek twice", test_peek_twice);
  SHOULDF("collect", test_collect);
  RETURN();
}
