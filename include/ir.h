#pragma once
#include "ast.h"
#include "pros.h"
#include "stdint.h"
#include "stdio.h"

typedef struct {

} pro_rets;

typedef struct {

} pro_source_t;

pro_source_t pro_new();
void pro_begin(pro_source_t *pro, ast_t *main);
void pro_add(pro_source_t *pro, ast_t *next);

void pro_clean(pro_source_t *pro);
void pro_iconst64(pro_source_t *source, uint64_t val);
