#define MORE_COFFEE
#define WITH_MOCKING
#include "../barista.h"
#include "../include/token.h"

#define EASY_LENGTH 53
static token_e easy_list[] = {
    OParen,      CParen,   OBrace, CBrace, OArray,   CArray,   Dot,    Comma,
    Dollar,      Question, Pound,  Colon,  SColon,   Backtick, At,     Lt,
    LtEq,        Gt,       GtEq,   Div,    BSlash,   Plus,     Rest,   Sub,
    Mul,         Or,       And,    Xor,    LShift,   RShift,   Not,    As,
    NotAs,       OrAs,     AndAs,  XorAs,  LShiftAs, RShiftAs, AndLog, OrLog,
    NotEquality, Equality, NotLog, Mod,    Inc,      Dec,      AddAs,  SubAs,
    DivAs,       MulAs,    ModAs,  Error,  Wsp};

static char *easy_strings[] = {
    "(",  ")",  "{",  "}",   "[",   "]",  ".",  ",",  "$",  "?",  "#",
    ":",  ";",  "`",  "@",   "<",   "<=", ">",  ">=", "/",  "\\", "+",
    "_",  "-",  "*",  "||",  "&&",  "^",  "<<", ">>", "!",  "=",  "~=",
    "|=", "&=", "^=", "<<=", ">>=", "&",  "|",  "!=", "==", "~",  "%",
    "++", "--", "+=", "-=",  "/=",  "*=", "%=", "",  "   "};

void test_nums() {
  char *chars = "72";
  int len = 0;
  token_e token = token_next(chars, &len);
  ASSERT(token == Num);
  ASSERT(len == 2);

  char *chars2 = "0x00";
  token = token_next(chars2, &len);
  ASSERT(token == Hex);
  ASSERT(len == 4);

  char *chars3 = "0x";
  token = token_next(chars3, &len);
  ASSERT(token == Error);
  ASSERT(len == 2);

  char *chars4 = "7.0";
  token = token_next(chars4, &len);
  ASSERT(token == Decimal);
  ASSERT(len == 3);

  char *chars5 = "17.0 ";
  token = token_next(chars5, &len);
  ASSERT(token == Decimal);
  ASSERT(len == 4);

  char *chars6 = "0";
  token = token_next(chars6, &len);
  ASSERT(token == Num);
  ASSERT(len == 1);

  char *chars7 = "0b001 ";
  token = token_next(chars7, &len);
  ASSERT(token == Bin);
  ASSERT(len == 5);

  char *chars8 = "0x0aF ";
  token = token_next(chars8, &len);
  ASSERT(token == Hex);
  ASSERT(len == 5);

  char *chars9 = "0 ";
  token = token_next(chars9, &len);
  ASSERT(token == Num);
  ASSERT(len == 1);
}

void test_chars() {
  char *chars = "'a'";
  int len = 0;
  token_e token = token_next(chars, &len);
  ASSERT(token == SQuote);
  ASSERT(len == 1);

  char *chars2 = "'\\n' ";
  token = token_next(chars2, &len);
  ASSERT(token == SQuote);
  ASSERT(len == 2);

  char *chars3 = "'n";
  token = token_next(chars3, &len);
  ASSERT(token == Error);
  ASSERT(len == 1);

  char *chars4 = "'\\";
  token = token_next(chars4, &len);
  ASSERT(token == Error);
  ASSERT(len == 1);
}

void test_strings() {
  char *strings = "\"hello\"";
  int len = 0;
  token_e token = token_next(strings, &len);
  ASSERT(token == DQuote);
  ASSERT(len == 5);

  char *strings2 = "\"henl\\nlo\" ";
  token = token_next(strings2, &len);
  ASSERT(token == DQuote);
  ASSERT(len == 8);

  char *strings3 = "\"henl";
  token = token_next(strings3, &len);
  ASSERT(token == Error);
  ASSERT(len == 4);
}

void test_easy() {
  for (int i = 0; i < EASY_LENGTH; i++) {
    int len = 0;
    token_e token = token_next(easy_strings[i], &len);
    ASSERT(token == (token_e)easy_list[i]);
    ASSERT(len == (int)strlen(easy_strings[i]));
  }
}

void test_keywords() {
  char *ret = "return 7;";
  int len = 0;
  token_e tok = token_next(ret, &len);
  ASSERT(tok == Return);
  for (size_t i = 0; i < KEYWORD_LENGTH; i++) {
    int len = 0;
    token_e token = token_next(keyword_list[i], &len);
    ASSERT((size_t)token == i);
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
  int len = 0;
  token_e token = token_next(null_end, &len);
  ASSERT(token == Symbol);
  token = token_next(underscore, &len);
  ASSERT(token == Symbol);
  token = token_next(func, &len);
  ASSERT(token == Symbol);
  token = token_next(dash, &len);
  ASSERT(token == Symbol);
}

int main() {
  DESCRIBE("token");
  SHOULDB("get string len for all tokens and match", {
    for (size_t i = 0; i < KEYWORD_LENGTH; i++) {
      ASSERT((size_t)strlen(keyword_list[i]) == (size_t)keyword_len[i]);
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

  SHOULDF("test easy", test_easy);
  SHOULDB("return an empty token_t on empty", {
    char *empty = "";
    int len = 0;
    token_e token = token_next(empty, &len);
    ASSERT(token == Empty);
  });
  SHOULDB("return an empty token_t at end", {
    char *end_test = "import";
    int len = 0;
    token_e token = token_next(end_test, &len);
    end_test += len;
    token = token_next(end_test, &len);
    ASSERT(token == Empty);
  });
  SHOULDF("test symbols", test_symbols);
  SHOULDF("test len words", test_len_words);
  SHOULDF("test keywords", test_keywords);
  SHOULDF("test chars", test_chars);
  SHOULDF("test strings", test_strings);
  SHOULDF("test numbers", test_nums);
  RETURN();
}
