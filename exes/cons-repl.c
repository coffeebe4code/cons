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
#include "inttypes.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define USER_SIZE 128

option_t opts[2] = {{"test", arg_none, 't'}, opt_end};

int main(int argc __attribute__((unused)),
         char **argv __attribute__((unused))) {
  // cli_parse(opts, argc, argv);
  char input[USER_SIZE] = {0};
  int cont = 1;

  while (cont) {
    lex_source_t lex_source;
    parser_source_t parse_source;
    ir_source_t ir_source;
    if (fgets(input, USER_SIZE, stdin) == NULL) {
      ERROR(&(cons_error_t){memory, {0}, 0}, "fgets failed in repl");
      exit(1);
    }
    if (input[0] == '/' && input[1] == '/') {
      // parse a repl command
      // quit or q, write or w, clear or c
    } else {
      lex_source = lex_new(input);
      parse_source = parser_new();
      ir_source = ir_new();
      ast_t *new_ast = parse_low_bin(&lex_source, &parse_source);
      ir_begin(&ir_source, new_ast);

      ir_flush_gen(&ir_source);
      gen_add32(&ir_source.gen, make_gen_instr(Ret, ir_source.main_exit, 0, 0));
      vm_t vm = vm_new(ir_source.gen.binary);
      vm = vm_run(vm);
      printf("result = %" PRIu64 "\n", vm.result);
      ir_free(&ir_source);
      parser_free(&parse_source);
    }
  }

  return 0;
}
