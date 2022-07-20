#include "../../include/error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *error_kinds[7] = {"scanner",  "lexer",  "parser", "linter",
                               "segfault", "memory", "runtime"};

void BUILDER(FILE *stream, char *tag, char *kind, char *fmt, va_list args) {
  fprintf(stream, "[%s] %s | ", tag, kind);
  vfprintf(stream, fmt, args);
  fprintf(stream, "\n");
}

void POINTER(FILE *stream, char *source_message, int pointer_loc, char *tag,
             char *kind, char *fmt, va_list args) {
  int tag_len = strlen(tag);
  int kind_len = strlen(kind);
  int empty_len = tag_len + kind_len + 5;
  char *empty = calloc(empty_len, sizeof(char));
  memset(empty, ' ', empty_len);
  fprintf(stream, "%s| ", empty);
  char *empty2 = calloc(empty_len + pointer_loc, sizeof(char));
  memset(empty2, ' ', empty_len + pointer_loc + 2);
  vfprintf(stream, fmt, args);
  fprintf(stream, "\n");
  fprintf(stream, "%s^ %s\n", empty2, source_message);
  free(empty);
  free(empty2);
}

void ERROR(cons_error_t *err, char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  BUILDER(stderr, "ERROR", error_kinds[err->err_kind], fmt, args);
  va_end(args);
}

void ERROR_PTR(cons_error_t *err, char *message, int pointer_loc, char *fmt,
               ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "[ERROR] %s => file: %s => line: %d\n",
          error_kinds[err->err_kind], err->first.file_name, err->line_no);
  POINTER(stderr, message, pointer_loc, "ERROR", error_kinds[err->err_kind],
          fmt, args);
  va_end(args);
}
