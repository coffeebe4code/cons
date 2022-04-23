#include "../../include/ast.h"
#include "../../include/lex.h"
#include "../../include/option.h"

int parse_lit(lex_source_t *lexer) {
  token_e peeked = lex_peek(lexer).tok;
  if (is_lit(peeked)) {
    return 1;
  }
  return 0;
}

int parse_un(lex_source_t *lexer) {
  token_e peeked = lex_peek(lexer).tok;
  if (is_un_op(peeked)) {
    return 1;
  }
  return 0;
}
