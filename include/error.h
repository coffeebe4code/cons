#pragma once
#include <stdio.h>

typedef enum {
  scanner = 0,
  lexer,
  parser,
  linter,
  segfault,
  memory,
  runtime
} error_type_e;

typedef struct {
  error_type_e err_kind;
} error_t;

#define ERR_FORMAT(STRING_INDEX, FIRST_TO_CHECK)                               \
  __attribute__((format(printf, STRING_INDEX, FIRST_TO_CHECK)))

void BUILDER(FILE *stream, char *tag, char *kind, char *fmt, va_list args);
void ERROR(error_t *err, char *fmt, ...);
void ERROR_PTR(error_t *err, char *message, int pointer_loc, char *fmt, ...);
