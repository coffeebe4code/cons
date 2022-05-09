#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/parse.h"
#include "../nobuild.h"

DECLARE_MOCK(lexeme_t, lex_peek, lex_source_t *lex);
DECLARE_MOCK(lexeme_t, lex_collect, lex_source_t *lex);
DECLARE_MOCK(int, is_lit, token_e t);
DECLARE_MOCK(int, is_un_op, token_e t);

int main() {
  DESCRIBE("parse");
  SHOULDB("work", { ASSERT(1 == 1); });
  RETURN();
}
