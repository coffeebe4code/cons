#define NOBUILD_IMPLEMENTATION
#define CFLAGS "-Wall", "-Werror", "-std=c11", "-Wextra"
#include "./nobuild.h"

int main(int argc, char **argv) {
  FEATURE("token");
  FEATURE("span");
  FEATURE("lex");
  FEATURE("parse");
  FEATURE("arch");
  DEPS("lex", "token", "span");
  DEPS("parse", "lex", "token", "span");
  BOOTSTRAP(argc, argv);
  return 0;
}
