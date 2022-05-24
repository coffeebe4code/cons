#pragma once
#include "../include/span.h"
#include "../include/token.h"

typedef struct {
  token_e tok;
  span_t span;
} lexeme_t;
