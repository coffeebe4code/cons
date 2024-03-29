#pragma once
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define LIST_DECL(type, name)                                                  \
  typedef struct {                                                             \
    size_t len;                                                                \
    size_t cap;                                                                \
    type *data;                                                                \
  } name##_l;

#define LIST_USE(type, name, capacity)                                         \
  name##_l name##_new() {                                                      \
    name##_l val = {.len = 0, .cap = capacity, .data = NULL};                  \
    val.data = calloc(capacity, sizeof(type));                                 \
    return val;                                                                \
  }                                                                            \
                                                                               \
  int name##_add(name##_l *list, type val) {                                   \
    if (list->cap <= list->len) {                                              \
      type *temp = NULL;                                                       \
      list->cap <<= 1;                                                         \
      temp = realloc(list->data, list->cap * sizeof(type));                    \
      if (temp == NULL) {                                                      \
        list->cap >>= 1;                                                       \
        list->cap++;                                                           \
        temp = realloc(list->data, list->cap * sizeof(type));                  \
        if (temp == NULL) {                                                    \
          return 1;                                                            \
        }                                                                      \
      }                                                                        \
      list->data = temp;                                                       \
    }                                                                          \
    memcpy(&list->data[list->len++], &val, sizeof(type));                      \
    return 0;                                                                  \
  }                                                                            \
  void name##_free(name##_l *list) { free(list->data); }
