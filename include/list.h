#ifndef __LISTS__
#define __LISTS__
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define LIST_MAKE(type, name, capacity)                                        \
  typedef struct {                                                             \
    size_t len;                                                                \
    size_t cap;                                                                \
    type *data;                                                                \
  } name##_l;                                                                  \
                                                                               \
  name##_l name##_new() {                                                      \
    name##_l val = {.len = 0, .cap = capacity, .data = NULL};                  \
    val.data = calloc(sizeof(type), capacity);                                 \
    return val;                                                                \
  }                                                                            \
                                                                               \
  int name##_add(name##_l *list, type val) {                                   \
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
  }

#endif
