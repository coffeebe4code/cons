#ifndef NOBUILD_H_
#define NOBUILD_H_

#include "pthread.h"
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
typedef FILE *Fd;

#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
typedef pid_t Pid;
double main_utime = 0;
long int main_sutime = 0;
double cmd_utime = 0;
long int cmd_sutime = 0;

#define PATH_SEP "/"

#ifndef CFLAGS
#define CFLAGS "-Wall", "-Werror", "-std=c11"
#endif
#ifndef CC
#define CC "gcc"
#endif
#ifndef AR
#define AR "ar"
#endif
#ifndef RCOMP
#define RCOMP "-O3"
#endif
#ifndef DCOMP
#define DCOMP "-fsanitize=undefined,address", "-g", "-O0"
#endif
#ifndef LD
#define LD "ld"
#endif
#if defined(__GNUC__) || defined(__clang__)
// https://gcc.gnu.org/onlinedocs/gcc-4.7.2/gcc/Function-Attributes.html
#define NOBUILD_PRINTF_FORMAT(STRING_INDEX, FIRST_TO_CHECK)                    \
  __attribute__((format(printf, STRING_INDEX, FIRST_TO_CHECK)))
#else
#define NOBUILD_PRINTF_FORMAT(STRING_INDEX, FIRST_TO_CHECK)
#endif

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

typedef char *Cstr;
void INFO(Cstr fmt, ...) NOBUILD_PRINTF_FORMAT(1, 2);

#define LIST_DECL_NB(type, name)                                               \
  typedef struct {                                                             \
    size_t len;                                                                \
    size_t cap;                                                                \
    type *data;                                                                \
  } name##_l;

#define LIST_USE_NB(type, name, capacity)                                      \
  name##_l name##_new() {                                                      \
    name##_l val = {.len = 0, .cap = capacity, .data = NULL};                  \
    val.data = (type *)calloc(sizeof(type), capacity);                         \
    return val;                                                                \
  }                                                                            \
                                                                               \
  int name##_add(name##_l *list, type val) {                                   \
    if (list->cap <= list->len) {                                              \
      type *temp = NULL;                                                       \
      list->cap <<= 1;                                                         \
      temp = (type *)realloc(list->data, list->cap * sizeof(type));            \
      if (temp == NULL) {                                                      \
        list->cap >>= 1;                                                       \
        list->cap++;                                                           \
        temp = (type *)realloc(list->data, (list->cap) * sizeof(type));        \
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

#define LIST_STRUSE_NB(type, name, capacity)                                   \
  name##_l name##_str_new() {                                                  \
    name##_l val = {.len = 0, .cap = capacity, .data = NULL};                  \
    val.data = (type *)calloc(sizeof(type), capacity);                         \
    return val;                                                                \
  }                                                                            \
                                                                               \
  int name##_str_add(name##_l *list, type val) {                               \
    type data = (char *)calloc(sizeof(char), strlen(val));                     \
    strcpy(data, val);                                                         \
    if (list->cap <= list->len) {                                              \
      type *temp = NULL;                                                       \
      list->cap <<= 1;                                                         \
      temp = (type *)realloc(list->data, list->cap * sizeof(type));            \
      if (temp == NULL) {                                                      \
        list->cap >>= 1;                                                       \
        list->cap++;                                                           \
        temp = (type *)realloc(list->data, (list->cap) * sizeof(type));        \
        if (temp == NULL) {                                                    \
          return 1;                                                            \
        }                                                                      \
      }                                                                        \
      list->data = temp;                                                       \
    }                                                                          \
    list->data[list->len] = data;                                              \
    list->len++;                                                               \
    return 0;                                                                  \
  }                                                                            \
  void name##_free(name##_l *list) { free(list->data); }

// typedefs
LIST_DECL_NB(char *, cstr);
LIST_STRUSE_NB(char *, cstr, 5);
LIST_DECL_NB(cstr_l, cstrs);
LIST_USE_NB(cstr_l, cstrs, 5);
typedef struct {
  short failure_total;
  short passed_total;
} result_t;
typedef struct {
  Cstr *elems;
  size_t count;
} Cstr_Array;
typedef struct {
  Cstr_Array line;
} Cmd;
typedef struct {
  Cmd *elems;
  size_t count;
} Cmd_Array;
typedef struct {
  Cstr *feature;
  Cstr_Array *array;
} thread_data_t;

// statics
static int test_result_status __attribute__((unused)) = 0;
static struct option flags[] = {{"build", required_argument, 0, 'b'},
                                {"init", no_argument, 0, 'i'},
                                {"clean", no_argument, 0, 'c'},
                                {"exe", required_argument, 0, 'e'},
                                {"release", no_argument, 0, 'r'},
                                {"skip-tests", no_argument, 0, 's'},
                                {"remove-feature", required_argument, 0, 'x'},
                                {"add", required_argument, 0, 'a'},
                                {"debug", no_argument, 0, 'd'},
                                {"pack", optional_argument, 0, 'p'},
                                {"total-internal", optional_argument, 0, 't'},
                                {0}};

static int skip_tests = 0;
static result_t results = {0, 0};
static cstrs_l features = {0, 0, 0};
static cstrs_l deps = {0, 0, 0};
static Cstr_Array *exes = NULL;
static size_t exe_count = 0;

// forwards
Cstr_Array deps_get_manual(Cstr feature, Cstr_Array processed);
void initialize();
Cstr_Array incremental_build(Cstr parsed, Cstr_Array processed);
Cstr_Array cstr_array_concat(Cstr_Array cstrs1, Cstr_Array cstrs2);
int cstr_ends_with(Cstr cstr, Cstr postfix);
Cstr cstr_no_ext(Cstr path);
Cstr_Array cstr_array_make(Cstr first, ...);
Cstr_Array cstr_array_append(Cstr_Array cstrs, Cstr cstr);
Cstr cstr_array_join(Cstr sep, Cstr_Array cstrs);
Fd fd_open_for_read(Cstr path, int exit);
Fd fd_open_for_write(Cstr path);
void fd_close(Fd fd);
void release();
void debug();
void build(Cstr_Array comp_flags);
void obj_build(Cstr feature, Cstr_Array comp_flags);
void obj_build_threaded(Cstr_Array comp_flags);
void test_build(Cstr feature, Cstr_Array comp_flags, Cstr_Array feature_links);
void exe_build(Cstr feature, Cstr_Array comp_flags, Cstr_Array deps);
Cstr_Array deps_get_lifted(Cstr file, Cstr_Array processed);
void manual_deps(Cstr feature, Cstr_Array deps);
void add_exe(Cstr_Array val);
void pid_wait(Pid pid);
void test_pid_wait(Pid pid);
int handle_args(int argc, char **argv);
void make_feature(char *val);
void make_exe(Cstr val);
void write_report();
void create_folders();
Cstr parse_feature_from_path(Cstr path);
Cstr cmd_show(Cmd cmd);
Pid cmd_run_async(Cmd cmd);
void cmd_run_sync(Cmd cmd);
void test_run_sync(Cmd cmd);
int path_is_dir(Cstr path);
void path_mkdirs(Cstr_Array path);
void path_rename(Cstr old_path, Cstr new_path);
void path_rm(Cstr path);
void VLOG(FILE *stream, Cstr tag, Cstr fmt, va_list args);
void TABLOG(FILE *stream, Cstr tag, Cstr fmt, va_list args);
void WARN(Cstr fmt, ...) NOBUILD_PRINTF_FORMAT(1, 2);
void ERRO(Cstr fmt, ...) NOBUILD_PRINTF_FORMAT(1, 2);
void PANIC(Cstr fmt, ...) NOBUILD_PRINTF_FORMAT(1, 2);
void FAILLOG(Cstr fmt, ...) NOBUILD_PRINTF_FORMAT(1, 2);
void DESCLOG(Cstr fmt, ...) NOBUILD_PRINTF_FORMAT(1, 2);
void RUNLOG(Cstr fmt, ...) NOBUILD_PRINTF_FORMAT(1, 2);
void OKAY(Cstr fmt, ...) NOBUILD_PRINTF_FORMAT(1, 2);

// macros
#define INITIALIZE()                                                           \
  do {                                                                         \
    features = cstrs_new();                                                    \
    deps = cstrs_new();                                                        \
  } while (0);

#define FOREACH_ARRAY(type, elem, array, body)                                 \
  for (size_t elem_##index = 0; elem_##index < array.count; ++elem_##index) {  \
    type *elem = &array.elems[elem_##index];                                   \
    body;                                                                      \
  }

#define CSTRS()                                                                \
  { .elems = NULL, .count = 0 }

#define ENDS_WITH(cstr, postfix) cstr_ends_with(cstr, postfix)
#define NOEXT(path) cstr_no_ext(path)
#define JOIN(sep, ...) cstr_array_join(sep, cstr_array_make(__VA_ARGS__, NULL))
#define CONCAT(...) JOIN("", __VA_ARGS__)
#define PATH(...) JOIN(PATH_SEP, __VA_ARGS__)

#define DEPS(first, ...)                                                       \
  do {                                                                         \
    cstr_l new_dep = cstr_str_new();                                           \
    cstr_l_add_varargs(&new_dep, __VA_ARGS__, NULL);                           \
    cstrs_add(&deps, new_dep);                                                 \
  } while (0)

#define EXE(...)                                                               \
  do {                                                                         \
    Cstr_Array exes_macro = cstr_array_make(__VA_ARGS__, NULL);                \
    add_exe(exes_macro);                                                       \
  } while (0)

#define CMD(...)                                                               \
  do {                                                                         \
    Cmd cmd = {.line = cstr_array_make(__VA_ARGS__, NULL)};                    \
    INFO("CMD: %s", cmd_show(cmd));                                            \
    cmd_run_sync(cmd);                                                         \
  } while (0)

#define REMOVE(feature)                                                        \
  do {                                                                         \
    RM(CONCAT("include/", feature, ".h"));                                     \
    RM(CONCAT("src/", feature));                                               \
    RM(CONCAT("tests/", feature, ".c"));                                       \
  } while (0)

#define CLEAN()                                                                \
  do {                                                                         \
    RM("target");                                                              \
    RM("obj");                                                                 \
  } while (0)

#define EXEC_TESTS(feature)                                                    \
  do {                                                                         \
    Cmd cmd = {                                                                \
        .line = cstr_array_make(CONCAT("target/", feature), NULL),             \
    };                                                                         \
    INFO("CMD: %s", cmd_show(cmd));                                            \
    test_run_sync(cmd);                                                        \
  } while (0)

#define RESULTS()                                                              \
  do {                                                                         \
    if (skip_tests == 0) {                                                     \
      update_results();                                                        \
      INFO("OKAY: tests passed %d", results.passed_total);                     \
      INFO("FAIL: tests failed %d", results.failure_total);                    \
      INFO(ANSI_COLOR_CYAN "TOTAL:" ANSI_COLOR_RESET " tests ran %d",          \
           results.failure_total + results.passed_total);                      \
      if (results.failure_total > 0) {                                         \
        exit(results.failure_total);                                           \
      }                                                                        \
    }                                                                          \
  } while (0)

#define FEATURE(...)                                                           \
  do {                                                                         \
    cstr_l new_feature = cstr_str_new();                                       \
    cstr_l_add_varargs(&new_feature, __VA_ARGS__, NULL);                       \
    cstrs_add(&features, new_feature);                                         \
  } while (0)

#define BOOTSTRAP(argc, argv)                                                  \
  do {                                                                         \
    handle_args(argc, argv);                                                   \
  } while (0)

#define IS_DIR(path) path_is_dir(path)

#define MKDIRS(...)                                                            \
  do {                                                                         \
    Cstr_Array path = cstr_array_make(__VA_ARGS__, NULL);                      \
    INFO("MKDIRS: %s", cstr_array_join(PATH_SEP, path));                       \
    path_mkdirs(path);                                                         \
  } while (0)

#define RM(path)                                                               \
  do {                                                                         \
    INFO("RM: %s", path);                                                      \
    path_rm(path);                                                             \
  } while (0)

#define FOREACH_FILE_IN_DIR(file, dirpath, body)                               \
  do {                                                                         \
    struct dirent *dp = NULL;                                                  \
    DIR *dir = opendir(dirpath);                                               \
    if (dir == NULL) {                                                         \
      PANIC("could not open directory %s: %d", dirpath, errno);                \
    }                                                                          \
    errno = 0;                                                                 \
    while ((dp = readdir(dir))) {                                              \
      if (strncmp(dp->d_name, ".", sizeof(char)) != 0) {                       \
        char *file = dp->d_name;                                               \
        body;                                                                  \
      }                                                                        \
    }                                                                          \
    if (errno > 0) {                                                           \
      PANIC("could not read directory %s: %d", dirpath, errno);                \
    }                                                                          \
    closedir(dir);                                                             \
  } while (0)

#endif // NOBUILD_H_

////////////////////////////////////////////////////////////////////////////////

#ifdef NOBUILD_IMPLEMENTATION

Cstr_Array cstr_array_append(Cstr_Array cstrs, Cstr cstr) {
  Cstr_Array result = {.count = cstrs.count + 1};
  result.elems = malloc(sizeof(result.elems[0]) * result.count);
  memcpy(result.elems, cstrs.elems, cstrs.count * sizeof(result.elems[0]));
  result.elems[cstrs.count] = cstr;
  return result;
}

int cstr_ends_with(Cstr cstr, Cstr postfix) {
  const size_t cstr_len = strlen(cstr);
  const size_t postfix_len = strlen(postfix);
  return postfix_len <= cstr_len &&
         strcmp(cstr + cstr_len - postfix_len, postfix) == 0;
}

Cstr cstr_no_ext(Cstr path) {
  size_t n = strlen(path);
  while (n > 0 && path[n - 1] != '.') {
    n -= 1;
  }

  if (n > 0) {
    char *result = malloc(n);
    memcpy(result, path, n);
    result[n - 1] = '\0';

    return result;
  } else {
    return path;
  }
}

void create_folders() {
  MKDIRS("target", "nobuild");
  MKDIRS("obj");
  for (size_t i = 0; i < features.len; i++) {
    MKDIRS(CONCAT("obj/", features.data[i].data[0]));
  }
}

void update_results() {
  for (size_t i = 0; i < features.len; i++) {
    Fd fd = fd_open_for_read(
        CONCAT("target/nobuild/", features.data[i].data[0], ".report"), 1);
    int number;
    if (fscanf((FILE *)fd, "%d", &number) == 0) {
      PANIC("couldn't read from file %s",
            CONCAT("target/nobuild/", features.data[i].data[0], ".report"));
    }
    results.passed_total += number;
    fclose(fd);
  }
}

void add_exe(Cstr_Array val) {
  if (exes == NULL) {
    exes = malloc(sizeof(Cstr_Array));
    exe_count++;
  } else {
    exes = realloc(exes, sizeof(Cstr_Array) * ++exe_count);
  }
  if (exes == NULL || val.count == 0) {
    PANIC("could not allocate memory: %s", strerror(errno));
  }
  memcpy(&exes[exe_count - 1], &val, sizeof(Cstr_Array));
}

void cstr_l_add_varargs(cstr_l *val, char *first, ...) {
  if (first == NULL) {
    return;
  }
  va_list args;
  va_start(args, first);
  cstr_str_add(val, first);
  for (char *next = va_arg(args, char *); next != NULL;
       next = va_arg(args, char *)) {
    cstr_str_add(val, next);
  }
  return;
}

Cstr_Array cstr_array_make(Cstr first, ...) {
  Cstr_Array result = CSTRS();
  size_t local_count = 0;
  if (first == NULL) {
    return result;
  }

  local_count += 1;
  va_list args;
  va_start(args, first);
  for (Cstr next = va_arg(args, Cstr); next != NULL;
       next = va_arg(args, Cstr)) {
    local_count += 1;
  }
  va_end(args);

  result.elems = calloc(local_count, sizeof(Cstr));
  if (result.elems == NULL) {
    PANIC("could not allocate memory: %s", strerror(errno));
  }
  result.count = 0;
  result.elems[result.count++] = first;

  va_start(args, first);
  for (Cstr next = va_arg(args, Cstr); next != NULL;
       next = va_arg(args, Cstr)) {
    result.elems[result.count++] = next;
  }
  va_end(args);

  return result;
}

Cstr_Array cstr_array_concat(Cstr_Array cstrs1, Cstr_Array cstrs2) {
  if (cstrs1.count == 0 && cstrs2.count == 0) {
    Cstr_Array temp = CSTRS();
    return temp;
  } else if (cstrs1.count == 0) {
    return cstrs2;
  } else if (cstrs2.count == 0) {
    return cstrs1;
  }

  cstrs1.elems =
      realloc(cstrs1.elems, sizeof(Cstr *) * (cstrs1.count + cstrs2.count));

  memcpy(&cstrs1.elems[cstrs1.count], &cstrs2.elems[0],
         sizeof(Cstr *) * cstrs2.count);
  cstrs1.count += cstrs2.count;
  return cstrs1;
}

Cstr cstr_array_join(Cstr sep, Cstr_Array cstrs) {
  if (cstrs.count == 0) {
    return "";
  }

  const size_t sep_len = strlen(sep);
  size_t len = 0;
  for (size_t i = 0; i < cstrs.count; ++i) {
    len += strlen(cstrs.elems[i]);
  }

  const size_t result_len = (cstrs.count - 1) * sep_len + len + 1;
  char *result = malloc(sizeof(char) * result_len);
  if (result == NULL) {
    PANIC("could not allocate memory: %s", strerror(errno));
  }

  len = 0;
  for (size_t i = 0; i < cstrs.count; ++i) {
    if (i > 0) {
      memcpy(result + len, sep, sep_len);
      len += sep_len;
    }

    size_t elem_len = strlen(cstrs.elems[i]);
    memcpy(result + len, cstrs.elems[i], elem_len);
    len += elem_len;
  }
  result[len] = '\0';

  return result;
}

Fd fd_open_for_read(Cstr path, int exit) {
  Fd result = fopen(path, "r+");
  if (result == NULL && exit) {
    PANIC("Could not open file %s: %d", path, errno);
  }
  return result;
}

Fd fd_open_for_write(Cstr path) {
  Fd result = fopen(path, "w+");
  if (result == NULL) {
    PANIC("could not open file %s: %d", path, errno);
  }
  return result;
}

void fd_close(Fd fd) { fclose(fd); }

void write_report(Cstr file) {
  Fd fd = fd_open_for_write(file);
  fprintf(fd, "%d", results.passed_total);
  fclose(fd);
}

int handle_args(int argc, char **argv) {
  int opt_char = -1;
  int found = 0;
  int option_index;
  int c = 0;
  int b = 0;
  int r = 0;
  int d = 0;
  char opt_b[256] = {0};

  while ((opt_char = getopt_long(argc, argv, "t:ce:sia:b:drx::", flags,
                                 &option_index)) != -1) {
    found = 1;
    switch ((int)opt_char) {
    case 's': {
      skip_tests = 1;
      break;
    }
    case 'x': {
      REMOVE(optarg);
      break;
    }
    case 'c': {
      c = 1;
      break;
    }
    case 'b': {
      strcpy(opt_b, optarg);
      b = 1;
      break;
    }
    case 'r': {
      r = 1;
      break;
    }
    case 'd': {
      d = 1;
      break;
    }
    case 'a': {
      make_feature(optarg);
      break;
    }
    case 'e': {
      make_exe(optarg);
      break;
    }
    case 'i': {
      initialize();
      break;
    }
    case 't': {
      break;
    }
    default: {
      break;
    }
    }
  }
  if (c) {
    CLEAN();
    create_folders();
  }
  if (b) {
    Cstr parsed = parse_feature_from_path(opt_b);
    Cstr_Array local_comp = cstr_array_make(DCOMP, NULL);
    Cstr_Array links = CSTRS();
    for (size_t j = 0; j < features.len; j++) {
      if (strcmp(parsed, features.data[j].data[0]) == 0) {
        for (size_t k = 1; k < features.data[j].len; k++) {
          links = cstr_array_append(links, features.data[j].data[k]);
        }

        obj_build(parsed, local_comp);
        test_build(parsed, local_comp, links);
        EXEC_TESTS(parsed);
        links.elems = NULL;
        links.count = 0;
      }
    }
    Cstr_Array exe_deps = CSTRS();
    for (size_t i = 0; i < exe_count; i++) {
      for (size_t k = 1; k < exes[i].count; k++) {
        exe_deps = cstr_array_append(links, exes[i].elems[k]);
      }
      exe_build(exes[i].elems[0], local_comp, exe_deps);
      exe_deps.elems = NULL;
      exe_deps.count = 0;
    }

    RESULTS();
  }
  if (r) {
    create_folders();
    release();
  }
  if (d) {
    create_folders();
    debug();
  }
  if (found == 0) {
    WARN("No arguments passed to nobuild");
    WARN("Building all features");
    create_folders();
    debug();
  }
  return 0;
}

void initialize() {
  create_folders();
  MKDIRS("exes");
  MKDIRS("src");
  MKDIRS("tests");
  MKDIRS("include");
  Cmd cmd = {.line = cstr_array_make(
                 "/bin/bash", "-c",
                 "echo -e '\n# nobuild\nnobuild\ntarget\ndeps\nobj\n' >> "
                 ".gitignore",
                 NULL)};
  cmd_run_sync(cmd);
}

void make_feature(Cstr feature) {
  Cstr inc = CONCAT("include/", feature, ".h");
  Cstr lib = CONCAT("src/", feature, "/lib.c");
  Cstr test = CONCAT("tests/", feature, ".c");
  MKDIRS("include");
  CMD("touch", inc);
  MKDIRS(CONCAT("src/", feature));
  CMD("touch", lib);
  MKDIRS("tests");
  CMD("touch", test);
}

void make_exe(Cstr val) {
  Cstr exe = CONCAT("exes/", val, ".c");
  MKDIRS("exes");
  CMD("touch", exe);
}

Cstr parse_feature_from_path(Cstr val) {
  Cstr noext = NOEXT(val);
  char *split = strtok((char *)noext, "/");
  if (strcmp(split, "tests") == 0 || strcmp(split, "include") == 0 ||
      strcmp(split, "src") == 0) {
    split = strtok(NULL, "/");
    return split;
  }
  size_t len = strlen(split);
  char *result = malloc(len * sizeof(char));
  memcpy(result, split, len);
  return result;
}

void test_pid_wait(Pid pid) {
  for (;;) {
    int wstatus = 0;
    if (waitpid(pid, &wstatus, 0) < 0) {
      PANIC("could not wait on command (pid %d): %d", pid, errno);
    }

    if (WIFEXITED(wstatus)) {
      int exit_status = WEXITSTATUS(wstatus);
      results.failure_total += exit_status;
      break;
    }

    if (WIFSIGNALED(wstatus)) {
      PANIC("command process was terminated by %d", WTERMSIG(wstatus));
    }
  }
}

void *obj_build_ptr(void *input) {
  thread_data_t *ptr = (thread_data_t *)input;
  obj_build(*ptr->feature, *ptr->array);
  return NULL;
}

void obj_build_threaded(Cstr_Array comp_flags) {
  pthread_t *tid = malloc(sizeof(pthread_t) * features.len);
  Cstr_Array links = CSTRS();
  for (size_t i = 0; i < features.len; i++) {
    for (size_t k = 1; k < features.data[i].len; k++) {
      links = cstr_array_append(links, features.data[i].data[k]);
    }
    thread_data_t *data = malloc(sizeof(thread_data_t));
    data->feature = &features.data[i].data[0];
    data->array = &comp_flags;
    pthread_create(&tid[i], NULL, obj_build_ptr, (void *)data);
    links.elems = NULL;
    links.count = 0;
  }
  for (size_t i = 0; i < features.len; i++) {
    pthread_join(tid[i], NULL);
  }
}

void obj_build(Cstr feature, Cstr_Array comp_flags) {
  FOREACH_FILE_IN_DIR(file, CONCAT("src/", feature), {
    Cstr output = CONCAT("obj/", feature, "/", NOEXT(file), ".o");
    Cmd obj_cmd = {.line = cstr_array_make(CC, CFLAGS, NULL)};
    obj_cmd.line = cstr_array_concat(obj_cmd.line, comp_flags);
    Cstr_Array arr = cstr_array_make("-fPIC", "-o", output, "-c", NULL);
    obj_cmd.line = cstr_array_concat(obj_cmd.line, arr);
    obj_cmd.line =
        cstr_array_append(obj_cmd.line, CONCAT("src/", feature, "/", file));
    INFO("CMD: %s", cmd_show(obj_cmd));
    cmd_run_sync(obj_cmd);
  });
}

Cstr_Array deps_get_manual(Cstr feature, Cstr_Array processed) {
  int proc_found = 0;
  for (size_t i = 0; i < processed.count; i++) {
    if (strcmp(processed.elems[i], feature) == 0) {
      proc_found += 1;
    }
  }
  if (proc_found == 0) {
    processed = cstr_array_append(processed, feature);
    for (size_t i = 0; i < deps.len; i++) {
      if (strcmp(deps.data[i].data[0], feature) == 0) {
        for (size_t j = 1; j < deps.data[i].len; j++) {
          int found = 0;
          for (size_t k = 0; k < processed.count; k++) {
            if (strcmp(processed.elems[k], deps.data[i].data[j]) == 0) {
              found += 1;
            }
          }
          if (found == 0) {
            processed = deps_get_manual(deps.data[i].data[j], processed);
          }
        }
      }
    }
  }
  return processed;
}

void test_build(Cstr feature, Cstr_Array comp_flags, Cstr_Array feature_links) {
  Cmd cmd = {.line = cstr_array_make(CC, CFLAGS, NULL)};
  cmd.line = cstr_array_concat(cmd.line, feature_links);
  cmd.line = cstr_array_concat(cmd.line, comp_flags);
  cmd.line = cstr_array_concat(
      cmd.line, cstr_array_make("-o", CONCAT("target/", feature),
                                CONCAT("tests/", feature, ".c"), NULL));

  FOREACH_FILE_IN_DIR(file, CONCAT("src/", feature), {
    Cstr output = CONCAT("obj/", feature, "/", NOEXT(file), ".o");
    cmd.line = cstr_array_append(cmd.line, output);
  });
  INFO("CMD: %s", cmd_show(cmd));
  cmd_run_sync(cmd);
}

void exe_build(Cstr exe, Cstr_Array comp_flags, Cstr_Array exe_deps) {
  Cmd cmd = {.line = cstr_array_make(CC, CFLAGS, NULL)};
  cmd.line = cstr_array_concat(cmd.line, comp_flags);
  Cstr_Array local_deps = CSTRS();
  Cstr_Array local_links = CSTRS();
  Cstr_Array output_list = CSTRS();
  for (size_t i = 0; i < exe_deps.count; i++) {
    local_deps = deps_get_manual(exe_deps.elems[i], local_deps);
  }
  for (size_t i = 0; i < local_deps.count; i++) {
    for (size_t k = 0; k < features.len; k++) {
      if (strcmp(local_deps.elems[i], features.data[k].data[0]) == 0) {
        for (size_t l = 1; l < features.data[k].len; l++) {
          local_links =
              cstr_array_append(local_links, features.data[k].data[l]);
        }
        FOREACH_FILE_IN_DIR(file, CONCAT("src/", features.data[k].data[0]), {
          Cstr output =
              CONCAT("obj/", features.data[k].data[0], "/", NOEXT(file), ".o");
          output_list = cstr_array_append(output_list, output);
        });
      }
    }
  }
  cmd.line = cstr_array_concat(cmd.line, local_links);
  cmd.line = cstr_array_concat(
      cmd.line, cstr_array_make("-o", CONCAT("target/", exe),
                                CONCAT("exes/", exe, ".c"), NULL));

  cmd.line = cstr_array_concat(cmd.line, output_list);
  INFO("CMD: %s", cmd_show(cmd));
  cmd_run_sync(cmd);
}

void release() { build(cstr_array_make(RCOMP, NULL)); }

Cstr_Array incremental_build(Cstr parsed, Cstr_Array processed) {
  processed = cstr_array_append(processed, parsed);
  for (size_t i = 0; i < deps.len; i++) {
    for (size_t j = 1; j < deps.data[i].len; j++) {
      if (strcmp(deps.data[i].data[j], parsed) == 0) {
        processed = incremental_build(deps.data[i].data[0], processed);
      }
    }
  }
  return processed;
}

void debug() { build(cstr_array_make(DCOMP, NULL)); }

void build(Cstr_Array comp_flags) {
  Cstr_Array links = CSTRS();
  obj_build_threaded(comp_flags);
  for (size_t i = 0; i < features.len; i++) {
    for (size_t k = 1; k < features.data[i].len; k++) {
      links = cstr_array_append(links, features.data[i].data[k]);
    }
    if (skip_tests == 0) {
      test_build(features.data[i].data[0], comp_flags, links);
      EXEC_TESTS(features.data[i].data[0]);
    }
    links.elems = NULL;
    links.count = 0;
  }
  Cstr_Array exe_deps = CSTRS();
  for (size_t i = 0; i < exe_count; i++) {
    for (size_t k = 1; k < exes[i].count; k++) {
      exe_deps = cstr_array_append(links, exes[i].elems[k]);
    }
    exe_build(exes[i].elems[0], comp_flags, exe_deps);
    exe_deps.elems = NULL;
    exe_deps.count = 0;
  }

  RESULTS();
}

void pid_wait(Pid pid) {
  for (;;) {
    int wstatus = 0;
    if (waitpid(pid, &wstatus, 0) < 0) {
      PANIC("could not wait on command (pid %d): %d", pid, errno);
    }

    if (WIFEXITED(wstatus)) {
      int exit_status = WEXITSTATUS(wstatus);
      if (exit_status != 0) {
        PANIC("command exited with exit code %d", exit_status);
      }
      break;
    }
    if (WIFSIGNALED(wstatus)) {
      PANIC("command process was terminated by %d", WTERMSIG(wstatus));
    }
  }
}

Cstr cmd_show(Cmd cmd) { return cstr_array_join(" ", cmd.line); }

Pid cmd_run_async(Cmd cmd) {
  pid_t cpid = fork();
  if (cpid < 0) {
    PANIC("Could not fork child process: %s: %s", cmd_show(cmd),
          strerror(errno));
  }
  if (cpid == 0) {
    Cstr_Array args = cstr_array_append(cmd.line, NULL);
    if (execvp(args.elems[0], (char *const *)args.elems) < 0) {
      PANIC("Could not exec child process: %s: %d", cmd_show(cmd), errno);
    }
  }
  return cpid;
}

void cmd_run_sync(Cmd cmd) { pid_wait(cmd_run_async(cmd)); }
void test_run_sync(Cmd cmd) { test_pid_wait(cmd_run_async(cmd)); }

int path_is_dir(Cstr path) {
  struct stat statbuf = {0};
  if (stat(path, &statbuf) < 0) {
    if (errno == ENOENT) {
      errno = 0;
      return 0;
    }

    PANIC("could not retrieve information about file %s: %s", path,
          strerror(errno));
  }

  return S_ISDIR(statbuf.st_mode);
}

void path_rename(char *old_path, char *new_path) {
  if (rename(old_path, new_path) < 0) {
    PANIC("could not rename %s to %s: %s", old_path, new_path, strerror(errno));
  }
}

void path_mkdirs(Cstr_Array path) {
  if (path.count == 0) {
    return;
  }

  size_t len = 0;
  for (size_t i = 0; i < path.count; ++i) {
    len += strlen(path.elems[i]);
  }

  size_t seps_count = path.count - 1;
  const size_t sep_len = strlen(PATH_SEP);

  char *result = malloc(len + seps_count * sep_len + 1);

  len = 0;
  for (size_t i = 0; i < path.count; ++i) {
    size_t n = strlen(path.elems[i]);
    memcpy(result + len, path.elems[i], n);
    len += n;

    if (seps_count > 0) {
      memcpy(result + len, PATH_SEP, sep_len);
      len += sep_len;
      seps_count -= 1;
    }

    result[len] = '\0';
    if (mkdir(result, 0755) < 0) {
      if (errno == EEXIST) {
        errno = 0;
      } else {
        PANIC("could not create directory %s: %s", result, strerror(errno));
      }
    }
  }
}

void path_rm(Cstr path) {
  if (IS_DIR(path)) {
    FOREACH_FILE_IN_DIR(file, path, {
      if (strcmp(file, ".") != 0 && strcmp(file, "..") != 0) {
        path_rm(PATH(path, file));
      }
    });

    if (rmdir(path) < 0) {
      if (errno == ENOENT) {
        errno = 0;
        WARN("directory %s does not exist", path);
      } else {
        PANIC("could not remove directory %s: %s", path, strerror(errno));
      }
    }
  } else {
    if (unlink(path) < 0) {
      if (errno == ENOENT) {
        errno = 0;
        WARN("file %s does not exist", path);
      } else {
        PANIC("could not remove file %s: %s", path, strerror(errno));
      }
    }
  }
}

void VLOG(FILE *stream, Cstr tag, Cstr fmt, va_list args) {
  fprintf(stream, "[%s] ", tag);
  vfprintf(stream, fmt, args);
  fprintf(stream, "\n");
}

void TABLOG(FILE *stream, Cstr tag, Cstr fmt, va_list args) {
  fprintf(stream, "      [%s] ", tag);
  vfprintf(stream, fmt, args);
  fprintf(stream, "\n");
}

void INFO(Cstr fmt __attribute__((unused)), ...) {
#ifndef NOINFO
  va_list args;
  va_start(args, fmt);
  VLOG(stderr, "INFO", fmt, args);
  va_end(args);
#endif
}

void OKAY(Cstr fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, ANSI_COLOR_GREEN "      [%s] " ANSI_COLOR_RESET, "OKAY");
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");
  va_end(args);
}

void DESCLOG(Cstr fmt, ...) {
  va_list args;
  va_start(args, fmt);
  VLOG(stderr, "DESC", fmt, args);
  va_end(args);
}

void FAILLOG(Cstr fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, ANSI_COLOR_RED "      [%s] " ANSI_COLOR_RESET, "FAIL");
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");
  va_end(args);
}

void RUNLOG(Cstr fmt, ...) {
  va_list args;
  va_start(args, fmt);
  TABLOG(stderr, "RUN!", fmt, args);
  va_end(args);
}

void WARN(Cstr fmt __attribute__((unused)), ...) {
#ifndef NOWARN
  va_list args;
  va_start(args, fmt);
  VLOG(stderr, "WARN", fmt, args);
  va_end(args);
#endif
}

void ERRO(Cstr fmt, ...) {
  va_list args;
  va_start(args, fmt);
  VLOG(stderr, "ERRO", fmt, args);
  va_end(args);
}

void PANIC(Cstr fmt, ...) {
  va_list args;
  va_start(args, fmt);
  VLOG(stderr, "ERRO", fmt, args);
  va_end(args);
  exit(1);
}

#endif // NOBUILD_IMPLEMENTATION
