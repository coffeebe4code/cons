#define CC "gcc"
#define CFLAGS "-Wall", "-Werror", "-std=c11", "-Wextra"
#include "./common.c"

int main(int argc, char **argv) {
  common_init();
  BOOTSTRAP(argc, argv);
  return 0;
}
