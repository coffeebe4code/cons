#include "../../include/lex.h"

lexeme_t lex_collect(lex_source_t *lex) {
  lexeme_t val;
  if ((int)lex->peeked_token == -1) {
    val = lex_peek(lex);
  } else {
    val = (lexeme_t){.tok = lex->peeked_token, .span = lex->peeked_span};
  }
  lex->peeked_token = (token_e)-1;
  lex->peeked_span = (span_t){NULL, 0};
  lex->curr_ptr += val.span.len;
  return val;
}

lexeme_t lex_peek(lex_source_t *lex) {
  int len = 0;
  if ((int)lex->peeked_token == -1) {
    lex->peeked_token = token_next(lex->curr_ptr, &len);
    lex->peeked_span = span_new(lex->curr_ptr, len);
  }
  lexeme_t val = {.tok = lex->peeked_token, .span = lex->peeked_span};
  return val;
}

lex_source_t lex_new(char *start) {
  return (lex_source_t){{NULL, 0}, start, -1};
}
