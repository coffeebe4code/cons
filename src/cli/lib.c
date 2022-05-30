#include "../../include/cli.h"
#include "stdio.h"
#include "stdlib.h"

int cli_parse(option_t *opts, int argc, char **argv) {
  int opt_count = 0;
  while (opts[opt_count++].arg != NULL) {
    printf("%s\n", opts[opt_count - 1].arg);
  }
  for (int i = 0; i < argc; i++) {
    printf("%s\n", argv[i]);
  }
  return 0;
}
