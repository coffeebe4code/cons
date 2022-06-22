#include "../include/cli.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define USER_SIZE 128

option_t opts[2] = {{"test", arg_none, 't'}, opt_end};

int main(int argc, char **argv) {
  cli_parse(opts, argc, argv);

  size_t size = USER_SIZE * 2;
  size_t idx = 0;
  char input[USER_SIZE] = {0};
  char *buffer = calloc(size, sizeof(char));
  int cont = 1;
  // print help text.
  //

  while (cont) {
    fgets(input, USER_SIZE, stdin);
    if (input[0] == '/' && input[1] == '/') {
      // parse a repl command
      // quit or q, write or w, clear or c
    } else {
      size_t len = strlen(input);
      printf("len %lu\n", len);
      size_t cmp = len + idx;
      printf("cmp %lu\n", cmp);
      if (cmp >= size) {
        size <<= 1;
        buffer = realloc(buffer, size);
      }
      memcpy(buffer + idx, input, len - 1);
      idx += len - 1;
    }
  }

  return 0;
}
