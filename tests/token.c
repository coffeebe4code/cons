#define NOBUILD_IMPLEMENTATION
#define WITH_MOCKING
#include "../include/token.h"
#include "../nobuild.h"

DECLARE_MOCK(span_t, span_new, char *data COMMA size_t len);

#define EASY_LENGTH 52
static token_e easy_list[] = {
    OParen,      CParen,   OBrace, CBrace, OArray,   CArray,   Dot,    Comma,
    Dollar,      Question, Pound,  Colon,  SColon,   Backtick, At,     Lt,
    LtEq,        Gt,       GtEq,   Div,    BSlash,   Plus,     Rest,   Sub,
    Mul,         Or,       And,    Xor,    LShift,   RShift,   Not,    As,
    NotAs,       OrAs,     AndAs,  XorAs,  LShiftAs, RShiftAs, AndLog, OrLog,
    NotEquality, Equality, NotLog, Mod,    Inc,      Dec,      AddAs,  SubAs,
    DivAs,       MulAs,    ModAs,  Error};

static char *easy_strings[] = {
    "(",  ")",  "{",  "}",   "[",   "]",  ".",  ",",  "$",  "?",  "#",
    ":",  ";",  "`",  "@",   "<",   "<=", ">",  ">=", "/",  "\\", "+",
    "_",  "-",  "*",  "||",  "&&",  "^",  "<<", ">>", "!",  "=",  "~=",
    "|=", "&=", "^=", "<<=", ">>=", "&",  "|",  "!=", "==", "~",  "%",
    "++", "--", "+=", "-=",  "/=",  "*=", "%=", ""};

void test_easy() {
  for (int i = 0; i < EASY_LENGTH; i++) {
    MOCK(span_new, (span_t){.ptr = NULL COMMA.len = 1});
    token_t token = token_next(easy_strings[i]);
    ASSERT(token.token == (token_e)easy_list[i]);
  }
}

void test_keywords() {
  for (size_t i = 0; i < KEYWORD_LENGTH; i++) {
    MOCK(span_new, (span_t){.ptr = keyword_list[i] COMMA.len = keyword_len[i]});
    token_t token = token_next(keyword_list[i]);
    ASSERT((size_t)token.token == i);
  }
}
void test_len_words() {
  char *null_end = "symbol1";
  char *underscore = "symbol_1";
  char *dash = "symbol-1;";
  char *func = "do_something()";
  size_t len = word_len_check(null_end);
  ASSERT(len == 7ul);
  len = word_len_check(underscore);
  ASSERT(len == 8ul);
  len = word_len_check(func);
  ASSERT(len == 12ul);
  len = word_len_check(dash);
  ASSERT(len == 8ul);
}

void test_symbols() {
  char *null_end = "symbol1";
  char *underscore = "symbol_1";
  char *dash = "symbol-1;";
  char *func = "do_something()";
  MOCK(span_new, (span_t){.ptr = null_end COMMA.len = 1});
  MOCK(span_new, (span_t){.ptr = underscore COMMA.len = 1});
  MOCK(span_new, (span_t){.ptr = func COMMA.len = 1});
  MOCK(span_new, (span_t){.ptr = dash COMMA.len = 1});
  token_t token = token_next(null_end);
  ASSERT(token.token == Symbol);
  token = token_next(underscore);
  ASSERT(token.token == Symbol);
  token = token_next(func);
  ASSERT(token.token == Symbol);
  token = token_next(dash);
  ASSERT(token.token == Symbol);
}

int main() {
  DESCRIBE("token");
  SHOULDB("get string len for all tokens and match", {
    for (size_t i = 0; i < KEYWORD_LENGTH; i++) {
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

  MOCK(span_new, (span_t){.ptr = NULL COMMA.len = 1});
  SHOULDF("test easy", test_easy);
  SHOULDB("return an empty token_t on empty", {
    char *empty = "";
    token_t token = token_next(empty);
    ASSERT(token.token == Empty);
  });
  SHOULDF("test symbols", test_symbols);
  SHOULDF("test len words", test_len_words);
  SHOULDF("test keywords", test_keywords);
  RETURN();
}
