#pragma once
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#define LIST_MAKE(type, name, capacity)                                        \
  typedef struct {                                                             \
    size_t len;                                                                \
    size_t cap;                                                                \
    type *data;                                                                \
  } name##_t;                                                                  \
  name##_t name##_new() {                                                      \
    name##_t val = {.len = 0, .cap = capacity, .data = NULL};                  \
    val.data = calloc(sizeof(type), capacity);                                 \
    return val;                                                                \
  }                                                                            \
  int name##_add(name##_t *list, type val) {                                   \
    if (list->cap <= list->len) {                                              \
      type *temp = NULL;                                                       \
      list->cap <<= 2;                                                         \
      temp = realloc(list->data, list->cap * sizeof(type));                    \
      if (temp == NULL) {                                                      \
        list->cap >>= 2;                                                       \
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
  }\
