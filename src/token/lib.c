#include "../../include/span.h"
#include "../../include/token.h"

// forwards
static inline token_t make_token(char *data, size_t len, token_e tok);
char *token_error(token_t *token);
token_t token_next(char *data);

token_t token_next(char *data) {
  int next = 1;
  int index = 0;
  token_t token;
  while (next && data[index] != '\0') {
    switch (data[index]) {
    case '+':
      token = make_token(data, index + 1, Plus);
      break;
    default:
      token = make_token(data, index, Error);
      break;
    }
  }
  return token;
}

static inline token_t make_token(char *data, size_t len, token_e tok) {
  token_t token = {.span = span_new(data, len), .token = tok};
  return token;
}
char *token_error(token_t *token) { return "error"; }
