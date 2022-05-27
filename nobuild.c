#define NOBUILD_IMPLEMENTATION
#define CC "clang"
#define CFLAGS "-Wall", "-Werror", "-std=c11", "-Wextra"
#include "./nobuild.h"

int main(int argc, char **argv) {
  FEATURE("token");
  FEATURE("span");
  FEATURE("lex");
  FEATURE("lexeme");
  FEATURE("ast");
  FEATURE("parse");
  FEATURE("bytecode");
  FEATURE("vm");
  FEATURE("gen");
  FEATURE("arch");
  FEATURE("uni");
  FEATURE("ir");
  FEATURE("byte");
  FEATURE("error");
  DEPS("lex", "lexeme", "token", "span");
  DEPS("lexeme", "token", "span");
  DEPS("ast", "lexeme", "token", "span");
  DEPS("parse", "ast", "lex", "lexeme", "span");
  DEPS("ir", "ast");
  DEPS("bytecode", "ast");
  BOOTSTRAP(argc, argv);
  return 0;
}
