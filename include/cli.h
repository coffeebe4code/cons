#pragma once

#define arg_none 0
#define arg_required 1
#define arg_optional 2
#define opt_end                                                                \
  (option_t) { NULL, 0, 'x' }

typedef struct option_t {
  const char *arg;
  int has_arg;
  char short_arg;
} option_t;

int cli_parse(option_t *opts, int argc, char **argv);
/* returns -1 when no more args, otherwise is the index of the option list
 * provided
 */
int cli_consume();
