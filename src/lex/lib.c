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

int is_num(token_e tok) {
  switch (tok) {
  case I32:
  case I16:
  case U32:
  case U64:
  case I64:
  case U16:
  case U8:
  case I8:
  case Bit:
  case F64:
  case F32:
  case D32:
  case D64:
  case Hex:
  case Bin:
  case Num:
    return 1;
    break;
  default:
    return 0;
    break;
  }
}

int is_lit(token_e tok) {
  switch (tok) {
  case Null:
  case True:
  case False:
  case SQuote:
  case DQuote:
  case Num:
    return 1;
    break;
  default:
    return 0;
    break;
  }
}

int is_low_bin(token_e tok) {
  switch (tok) {
  case Sub:
  case Plus:
    return 1;
    break;
  default:
    return 0;
    break;
  }
}

int is_high_bin(token_e tok) {
  switch (tok) {
  case Div:
  case Mul:
  case Mod:
    return 1;
    break;
  default:
    return 0;
    break;
  }
}

int is_bin_op(token_e tok) {
  switch (tok) {
  case Lt:
  case LtEq:
  case Gt:
  case GtEq:
  case Div:
  case Sub:
  case Mul:
  case Or:
  case And:
  case Xor:
  case LShift:
  case RShift:
  case AndLog:
  case OrLog:
  case NotEquality:
  case Equality:
  case Mod:
    return 1;
    break;
  default:
    return 0;
    break;
  }
}

int is_un_op(token_e tok) {
  switch (tok) {
  case Not:
  case Inc:
  case NotLog:
  case Addr:
  case Ptr:
  case Dec:
    return 1;
    break;
  default:
    return 0;
    break;
  }
}

int is_rh_assoc(token_e tok) {
  switch (tok) {
  case Inc:
  case Dec:
  case NotLog:
  case As:
  case AddAs:
  case SubAs:
  case MulAs:
  case DivAs:
  case ModAs:
  case OrAs:
  case AndAs:
  case XorAs:
  case LShiftAs:
  case RShiftAs:
    return 1;
    break;
  default:
    return 0;
    break;
  }
}

int is_lh_assoc(token_e tok) { return !is_rh_assoc(tok); }
