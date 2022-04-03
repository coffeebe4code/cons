#include "../../include/token.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// forwards

static inline token_e tokenize_lt(char *data, int *len) {
  token_e token;
  if (*++data != '\0') {
    switch (*data) {
    case '<':
      if (*++data != '\0' && *data == '=') {
        *len = 3;
        token = LShiftAs;
      } else {
        *len = 2;
        token = LShift;
      }
      break;
    case '=':
      *len = 2;
      token = LtEq;
      break;
    default:
      *len = 1;
      token = Lt;
      break;
    }
  } else {
    *len = 1;
    token = Lt;
  }
  return token;
}

static inline token_e tokenize_gt(char *data, int *len) {
  token_e token;
  if (*++data != '\0') {
    switch (*data) {
    case '>':
      if (*++data != '\0' && *data == '=') {
        *len = 3;
        token = RShiftAs;
      } else {
        *len = 2;
        token = RShift;
      }
      break;
    case '=':
      *len = 2;
      token = GtEq;
      break;
    default:
      *len = 1;
      token = Gt;
      break;
    }
  } else {
    *len = 1;
    token = Gt;
  }
  return token;
}

static inline token_e tokenize_one(char *data, int *len, token_e def_tok,
                                   const char comp, token_e comp_tok) {
  token_e token;
  if (*++data != '\0') {
    if (*data == comp) {
      *len = 2;
      token = comp_tok;
    } else {
      *len = 1;
      token = def_tok;
    }
  } else {
    *len = 1;
    token = def_tok;
  }
  return token;
}

static inline token_e tokenize_two(char *data, int *len, token_e def_tok,
                                   const char comp1, token_e comp1_tok,
                                   const char comp2, token_e comp2_tok) {
  token_e token;
  if (*++data != '\0') {
    if (*data == comp1) {
      *len = 2;
      token = comp1_tok;
    } else if (*data == comp2) {
      *len = 2;
      token = comp2_tok;
    } else {
      *len = 1;
      token = def_tok;
    }
  } else {
    *len = 1;
    token = def_tok;
  }
  return token;
}

inline size_t word_len_check(char *data) {
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

static inline token_e tokenize_word(char *data, int *len) {
  token_e token;
  *len = word_len_check(data);
  token = Symbol;
  for (size_t i = 0; i < KEYWORD_LENGTH; i++) {
    if (keyword_len[i] == *len) {
      if (strncmp(keyword_list[i], data, *len) == 0) {
        token = (token_e)i;
      }
    }
  }
  return token;
}

static inline size_t skip_whitespace(char *data) {
  size_t len = 1;
  while (*++data != '\0') {
    if (*data == ' ') {
      len++;
    } else {
      break;
    }
  }
  return len;
}

token_e token_next(char *data, int *len) {
  token_e token;
  if (isalpha(*data)) {
    token = tokenize_word(data, len);
  } else {
    switch (*data) {
    case ' ':
      token = Wsp;
      *len = (int)skip_whitespace(data);
      break;
    case '(':
      *len = 1;
      token = OParen;
      break;
    case ')':
      *len = 1;
      token = CParen;
      break;
    case '{':
      *len = 1;
      token = OBrace;
      break;
    case '}':
      *len = 1;
      token = CBrace;
      break;
    case '[':
      *len = 1;
      token = OArray;
      break;
    case ']':
      *len = 1;
      token = CArray;
      break;
    case '.':
      *len = 1;
      token = Dot;
      break;
    case ',':
      *len = 1;
      token = Comma;
      break;
    case '$':
      *len = 1;
      token = Dollar;
      break;
    case '?':
      *len = 1;
      token = Question;
      break;
    case '#':
      *len = 1;
      token = Pound;
      break;
    case ':':
      *len = 1;
      token = Colon;
      break;
    case ';':
      *len = 1;
      token = SColon;
      break;
    case '_':
      *len = 1;
      token = Rest;
      break;
    case '\\':
      *len = 1;
      token = BSlash;
      break;
    case '`':
      *len = 1;
      token = Backtick;
      break;
    case '@':
      *len = 1;
      token = At;
      break;
    case '>':
      token = tokenize_gt(data, len);
      break;
    case '|':
      token = tokenize_two(data, len, OrLog, '=', OrAs, '|', Or);
      break;
    case '&':
      token = tokenize_two(data, len, AndLog, '&', And, '=', AndAs);
      break;
    case '<':
      token = tokenize_lt(data, len);
      break;
    case '+':
      token = tokenize_two(data, len, Plus, '+', Inc, '=', AddAs);
      break;
    case '-':
      token = tokenize_two(data, len, Sub, '-', Dec, '=', SubAs);
      break;
    case '/':
      token = tokenize_one(data, len, Div, '=', DivAs);
      break;
    case '*':
      token = tokenize_one(data, len, Mul, '=', MulAs);
      break;
    case '^':
      token = tokenize_one(data, len, Xor, '=', XorAs);
      break;
    case '!':
      token = tokenize_one(data, len, Not, '=', NotEquality);
      break;
    case '%':
      token = tokenize_one(data, len, Mod, '=', ModAs);
      break;
    case '~':
      token = tokenize_one(data, len, NotLog, '=', NotAs);
      break;
    case '=':
      token = tokenize_one(data, len, As, '=', Equality);
      break;
    case '\n':
      token = NewLine;
      *len = 1;
      break;
    case EOF || '\0':
      *len = 0;
      token = Empty;
      break;
    default:
      if (strcmp(data, "") == 0) {
        *len = 0;
        token = Empty;
      } else {
        *len = 1;
        token = Error;
      }
      break;
    }
  }
  return token;
}
