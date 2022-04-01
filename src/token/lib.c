#include "../../include/token.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// forwards
static inline token_t make_token(char *data, size_t len, token_e tok);

static inline token_t tokenize_lt(char *data) {
  char *start = data;
  token_t token;
  if (*++data != '\0') {
    switch (*data) {
    case '<':
      if (*++data != '\0' && *data == '=') {
        token = make_token(start, 3, LShiftAs);
      } else {
        token = make_token(start, 2, LShift);
      }
      break;
    case '=':
      token = make_token(start, 2, LtEq);
      break;
    default:
      token = make_token(start, 1, Lt);
      break;
    }
  } else {
    token = make_token(start, 1, Lt);
  }
  return token;
}

static inline token_t tokenize_gt(char *data) {
  char *start = data;
  token_t token;
  if (*++data != '\0') {
    switch (*data) {
    case '>':
      if (*++data != '\0' && *data == '=') {
        token = make_token(start, 3, RShiftAs);
      } else {
        token = make_token(start, 2, RShift);
      }
      break;
    case '=':
      token = make_token(start, 2, GtEq);
      break;
    default:
      token = make_token(start, 1, Gt);
      break;
    }
  } else {
    token = make_token(start, 1, Gt);
  }
  return token;
}

static inline token_t tokenize_one(char *data, token_e def_tok, const char comp,
                                   token_e comp_tok) {
  char *start = data;
  token_t token;
  if (*++data != '\0') {
    if (*data == comp) {
      token = make_token(start, 2, comp_tok);
    } else {
      token = make_token(start, 1, def_tok);
    }
  } else {
    token = make_token(start, 1, def_tok);
  }
  return token;
}

static inline token_t tokenize_two(char *data, token_e def_tok,
                                   const char comp1, token_e comp1_tok,
                                   const char comp2, token_e comp2_tok) {
  char *start = data;
  token_t token;
  if (*++data != '\0') {
    if (*data == comp1) {
      token = make_token(start, 2, comp1_tok);
    } else if (*data == comp2) {
      token = make_token(start, 2, comp2_tok);
    } else {
      token = make_token(start, 1, def_tok);
    }
  } else {
    token = make_token(start, 1, def_tok);
  }
  return token;
}

size_t word_len_check(char *data) {
  int next = 1;
  size_t len = 1;
  while (*++data != '\0' && next) {
    if (isalnum(*data)) {
      len++;
    } else {
      switch (*data) {
      case '_':
      case '-':
        len++;
        break;
      default:
        next = 0;
        break;
      }
    }
  }
  return len;
}

static inline token_t tokenize_word(char *data) {
  char *start = data;
  token_t token;
  size_t len = word_len_check(data);
  token = make_token(start, len, Symbol);
  for (size_t i = 0; i < KEYWORD_LENGTH; i++) {
    if (keyword_len[i] == len) {
      if (strncmp(keyword_list[i], data, len) == 0) {
        token.token = (token_e)i;
      }
    }
  }
  return token;
}

token_t token_next(char *data) {
  token_t token;
  if (isalpha(*data)) {
    token = tokenize_word(data);
  } else {
    switch (*data) {
    case '(':
      token = make_token(data, 1, OParen);
      break;
    case ')':
      token = make_token(data, 1, CParen);
      break;
    case '{':
      token = make_token(data, 1, OBrace);
      break;
    case '}':
      token = make_token(data, 1, CBrace);
      break;
    case '[':
      token = make_token(data, 1, OArray);
      break;
    case ']':
      token = make_token(data, 1, CArray);
      break;
    case '.':
      token = make_token(data, 1, Dot);
      break;
    case ',':
      token = make_token(data, 1, Comma);
      break;
    case '$':
      token = make_token(data, 1, Dollar);
      break;
    case '?':
      token = make_token(data, 1, Question);
      break;
    case '#':
      token = make_token(data, 1, Pound);
      break;
    case ':':
      token = make_token(data, 1, Colon);
      break;
    case ';':
      token = make_token(data, 1, SColon);
      break;
    case '_':
      token = make_token(data, 1, Rest);
      break;
    case '\\':
      token = make_token(data, 1, BSlash);
      break;
    case '`':
      token = make_token(data, 1, Backtick);
      break;
    case '@':
      token = make_token(data, 1, At);
      break;
    case '>':
      token = tokenize_gt(data);
      break;
    case '|':
      token = tokenize_two(data, OrLog, '=', OrAs, '|', Or);
      break;
    case '&':
      token = tokenize_two(data, AndLog, '&', And, '=', AndAs);
      break;
    case '<':
      token = tokenize_lt(data);
      break;
    case '+':
      token = tokenize_two(data, Plus, '+', Inc, '=', AddAs);
      break;
    case '-':
      token = tokenize_two(data, Sub, '-', Dec, '=', SubAs);
      break;
    case '/':
      token = tokenize_one(data, Div, '=', DivAs);
      break;
    case '*':
      token = tokenize_one(data, Mul, '=', MulAs);
      break;
    case '^':
      token = tokenize_one(data, Xor, '=', XorAs);
      break;
    case '!':
      token = tokenize_one(data, Not, '=', NotEquality);
      break;
    case '%':
      token = tokenize_one(data, Mod, '=', ModAs);
      break;
    case '~':
      token = tokenize_one(data, NotLog, '=', NotAs);
      break;
    case '=':
      token = tokenize_one(data, As, '=', Equality);
      break;
    case EOF || '\0':
      token = (token_t){Empty, {NULL, 0}};
      break;
    default:
      if (strcmp(data, "") == 0) {
        token = (token_t){Empty, {NULL, 0}};
      } else {
        token = make_token(data, 1, Error);
      }
      break;
    }
  }
  return token;
}

static inline token_t make_token(char *start, size_t len, token_e tok) {
  token_t token = {.span = span_new(start, len), .token = tok};
  return token;
}

char *token_error(token_t *token) { return "error"; }
