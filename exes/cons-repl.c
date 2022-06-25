#include "../include/ast.h"
#include "../include/cli.h"
#include "../include/error.h"
#include "../include/ir.h"
#include "../include/lex.h"
#include "../include/lexeme.h"
#include "../include/parse.h"
#include "../include/pros.h"
#include "../include/token.h"
#include "../include/vm.h"
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
  lex_source_t lex_source;
  parser_source_t parse_source = parser_new();
  ir_source_t ir_source = ir_new();
  int cont = 1;
  // print help text.
  //

  while (cont) {
    if (fgets(input, USER_SIZE, stdin) == NULL) {
      ERROR(&(error_t){memory, {0}, 0}, "fgets failed in repl");
      exit(1);
    }
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
      lex_source = lex_new(buffer);
      ast_t *new_ast = parse_low_bin(&lex_source, &parse_source);
      ir_begin(&ir_source, new_ast);
      vm_t vm = vm_new(ir_source.gen.binary);
      vm_run(vm);
    }
  }

  return 0;
}
