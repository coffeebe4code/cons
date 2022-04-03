#include "../../include/lex.h"

lexed_t lex_collect(lex_source_t *lex) {
  lexed_t val = {.tok = lex->peeked_token, .span = lex->peeked_span};
  lex->peeked_token = (token_e)-1;
  lex->peeked_span = (span_t){NULL, 0};
  lex->curr_ptr += val.span.len;
  return val;
}

lexed_t lex_peek(lex_source_t *lex) {
  int len = 0;
  if ((int)lex->peeked_token == -1) {
    lex->peeked_token = token_next(lex->curr_ptr, &len);
    lex->peeked_span = span_new(lex->curr_ptr, len);
  }
  lexed_t val = {.tok = lex->peeked_token, .span = lex->peeked_span};
  return val;
}

lex_source_t lex_new(char *start) {
  return (lex_source_t){{NULL, 0}, start, -1};
}
