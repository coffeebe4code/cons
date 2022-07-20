#pragma once
#include "stdarg.h"
#include "stdio.h"

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
  union {
    char *file_name;
    char *message;
  } first;
  int line_no;
} cons_error_t;

#define ERR_FORMAT(STRING_INDEX, FIRST_TO_CHECK)                               \
  __attribute__((format(printf, STRING_INDEX, FIRST_TO_CHECK)))

void BUILDER(FILE *stream, char *tag, char *kind, char *fmt, va_list args);
void ERROR(cons_error_t *err, char *fmt, ...);
void ERROR_PTR(cons_error_t *err, char *message, int pointer_loc, char *fmt,
               ...);
