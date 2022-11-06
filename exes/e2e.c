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

static char *input = ""
                     "{\n"
                     "mut x = 21;\n"
                     "return x + x;\n"
                     "}\n";

int main() {

  lex_source_t lex_source;
  parser_source_t parse_source;
  ir_source_t ir_source;
  lex_source = lex_new(input);
  parse_source = parser_new();
  ir_source = ir_new();
  ast_t *body = parse_body(&lex_source, &parse_source);
  ir_main(&ir_source, body);

  ir_flush_gen(&ir_source);
  vm_t vm = vm_new(ir_source.gen.binary);
  vm = vm_run(vm);
  size_t result = vm.result;
  ir_free(&ir_source);
  parser_free(&parse_source);
  if (result == 42) {
    puts("success");
    return 0;
  }
  puts("fail");
  return 1;
}
