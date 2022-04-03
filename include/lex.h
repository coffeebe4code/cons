#pragma once
#include "span.h"
#include "token.h"

typedef struct {
  span_t peeked_span;
  char *curr_ptr;
  token_e peeked_token;
} lex_source_t;

typedef struct {
  token_e tok;
  span_t span;
} lexed_t;

lexed_t lex_collect(lex_source_t *lex);
lexed_t lex_peek(lex_source_t *lex);
lex_source_t lex_new(char *start);
int is_bin_op(token_e tok);
int is_un_op(token_e tok);
int get_precedence(token_e tok);
