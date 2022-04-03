#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/lex.h"
#include "../nobuild.h"

char *full_string = "#import whatever";
DECLARE_MOCK(span_t, span_new, char *data COMMA_D size_t len);
DECLARE_MOCK(token_e, token_next, char *data COMMA_D int *len);

void test_peek() {
  lex_source_t val = lex_new(full_string);
  MOCK(token_next, Pound);
  MOCK(span_new, (span_t){full_string COMMA 1});
  lexed_t peeked = lex_peek(&val);
  ASSERT(peeked.tok == Pound);
  ASSERT(val.curr_ptr == full_string);
  ASSERT(val.peeked_span.ptr == full_string);
  ASSERT(val.peeked_token == Pound);

  peeked = lex_peek(&val);
  ASSERT(peeked.tok == Pound);
  ASSERT(val.curr_ptr == full_string);
  ASSERT(val.peeked_span.ptr == full_string);
  ASSERT(val.peeked_token == Pound);
}
void test_collect() {
  lex_source_t val = lex_new(full_string);
  MOCK(token_next, Pound);
  MOCK(span_new, (span_t){full_string COMMA 1});
  lexed_t collect = lex_collect(&val);
  ASSERT(collect.tok == Pound);
  ASSERT(collect.span.ptr == full_string);
  MOCK(token_next, Import);
  MOCK(span_new, (span_t){full_string + 1 COMMA 6});
  lexed_t peeked = lex_peek(&val);
  ASSERT(val.curr_ptr == full_string + 1);
  ASSERT(peeked.tok == Import);
}
int main() {
  DESCRIBE("lex");
  SHOULDF("peek", test_peek);
  SHOULDF("collect", test_collect);
  RETURN();
}
