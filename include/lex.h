#pragma once
#include "lexeme.h"
#include "span.h"
#include "token.h"

typedef struct {
  span_t peeked_span;
  char *curr_ptr;
  token_e peeked_token;
} lex_source_t;

lexeme_t lex_collect(lex_source_t *lex);
lexeme_t lex_peek(lex_source_t *lex);
lex_source_t lex_new(char *start);
void lex_free(lex_source_t *lex);
int is_num(token_e tok);
int is_bin_op(token_e tok);
int is_high_bin(token_e tok);
int is_expr(token_e tok);
int is_un_op(token_e tok);
int is_lit(token_e tok);
int is_rh_assoc(token_e tok);
int is_lh_assoc(token_e tok);
int get_precedence(token_e tok);
