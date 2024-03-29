#define NOBUILD_IMPLEMENTATION
#include "./nobuild.h"

void common_init() {
  FEATURE("token");
  FEATURE("hash");
  FEATURE("list");
  FEATURE("span");
  FEATURE("lex");
  FEATURE("lexeme");
  FEATURE("ast");
  FEATURE("parse");
  FEATURE("vm");
  FEATURE("gen");
  FEATURE("config");
  FEATURE("ir");
  FEATURE("byte");
  FEATURE("error");
  FEATURE("cli");
  FEATURE("pros");
  DEPS("lex", "lexeme", "token", "span");
  DEPS("vm", "byte");
  DEPS("pros", "list", "byte");
  DEPS("lexeme", "token", "span");
  DEPS("ast", "lexeme", "token", "span");
  DEPS("parse", "ast", "lex", "lexeme", "span", "hash");
  DEPS("ir", "ast", "pros", "byte", "gen");
  EXE("cons-repl", "cli", "ast", "ir", "lex", "lexeme", "parse", "pros",
      "token", "vm", "error", "hash");
  EXE("e2e", "ast", "ir", "lex", "lexeme", "parse", "pros", "token", "vm",
      "hash");
}
