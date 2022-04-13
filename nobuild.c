#define NOBUILD_IMPLEMENTATION
#define CFLAGS "-Wall", "-Werror", "-std=c11", "-Wextra"
#include "./nobuild.h"

int main(int argc, char **argv) {
  FEATURE("token");
  FEATURE("span");
  FEATURE("lex");
  FEATURE("parse");
  FEATURE("gen");
  FEATURE("arch");
  FEATURE("uni");
  FEATURE("byte-bit");
  DEPS("lex", "token", "span");
  DEPS("parse", "lex", "token", "span");
  DEPS("arch", "gen");
  DEPS("byte-bit", "uni");
  BOOTSTRAP(argc, argv);
  return 0;
}
