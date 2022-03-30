#include "../../include/token.h"
#include <stdio.h>

// forwards
static inline token_t make_token(char *data, char *end, token_e tok);

token_t token_next(char *data) {
  char *start = data;
  token_t token;
  switch (*data) {
  case '+':
    token = make_token(start, start + sizeof(char), Plus);
    break;
  default:
    token = make_token(start, start + sizeof(char), Error);
    break;
  }
  return token;
}

static inline token_t make_token(char *start, char *end, token_e tok) {
  token_t token = {.span = span_new(start, (end - start) / sizeof(char)),
                   .token = tok};
  return token;
}
char *token_error(token_t *token) { return "error"; }
