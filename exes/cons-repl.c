#include "../include/cli.h"
#include "stdio.h"

option_t opts[2] = {{"test", arg_none, 't'}, opt_end};

int main(int argc, char **argv) {
  cli_parse(opts, argc, argv);

  return 0;
}
