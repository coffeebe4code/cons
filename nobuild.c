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
  FEATURE("vm");
  FEATURE("gen");
  FEATURE("arch");
  FEATURE("config");
  FEATURE("ir");
  FEATURE("byte");
  FEATURE("error");
  FEATURE("cli");
  FEATURE("pros");
  EXE("cons-repl", "cli");
  DEPS("lex", "lexeme", "token", "span");
  DEPS("lexeme", "token", "span");
  DEPS("ast", "lexeme", "token", "span");
  DEPS("parse", "ast", "lex", "lexeme", "span");
  DEPS("ir", "ast", "pros");
  BOOTSTRAP(argc, argv);
  return 0;
}
