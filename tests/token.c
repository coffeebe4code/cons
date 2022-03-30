#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/token.h"
#include "../nobuild.h"

DECLARE_MOCK(span_t, span_new, char *data Comma size_t len);

int main() {
  DESCRIBE("token");
  SHOULDB("get string len for all tokens and match", {
    for (size_t i = 0; i < TOKEN_LENGTH; i++) {
      ASSERT(strlen(keyword_list[i]) == keyword_len[i]);
    }
  });

  SHOULDB("spot check enum indexes", {
    ASSERT_STR_EQ(keyword_list[0], "import");
    ASSERT((token_e)0 == Import);

    ASSERT_STR_EQ(keyword_list[4], "mut");
    ASSERT((token_e)4 == Mut);

    ASSERT_STR_EQ(keyword_list[12], "u8");
    ASSERT((token_e)12 == U8);

    ASSERT_STR_EQ(keyword_list[20], "else");
    ASSERT((token_e)20 == Else);

    ASSERT_STR_EQ(keyword_list[30], "in");
    ASSERT((token_e)30 == In);

    ASSERT_STR_EQ(keyword_list[40], "match");
    ASSERT((token_e)40 == Match);

    ASSERT_STR_EQ(keyword_list[45], "void");
    ASSERT((token_e)45 == Void);
  });

  MOCK(span_new, (span_t){.ptr = NULL Comma.len = 1});
  SHOULDB("return all tokens correctly", {
    char *token_string = "+";
    token_t token = token_next(token_string);
    ASSERT(token.token == Plus);
    INFO("Complete %s", token_string);
  });
  RETURN();
}
