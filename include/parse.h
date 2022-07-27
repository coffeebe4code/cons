#pragma once
#include "ast.h"
#include "lex.h"

// double pointer structure. asts point to other asts.
// therefore cannot realloc the ast list and have them move.
// first pointer  **asts is for list of asts serially.
// var x;
// var y;
// the second pointer is for the root ast, which will have more ast nodes.
// ez_free contains a list of all pointers that have been malloc'd
typedef struct {
  ast_t **ez_free;
  ast_t **asts;
  size_t cap;
  size_t len;
  size_t free_cap;
  size_t free_len;
} parser_source_t;

parser_source_t parser_new();
void parser_free(parser_source_t *parser);
ast_t *parse_body(lex_source_t *lexer, parser_source_t *parser, int *start,
                  int *end);
ast_t *parse_statement(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_return(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_expr(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_inner_assign(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_reassign(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_or_log(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_and_log(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_comp(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_low_bin(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_high_bin(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_unary(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_terminal(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_num(lex_source_t *lexer, parser_source_t *parser);
ast_t *parse_ident(lex_source_t *lexer, parser_source_t *parser);
