#include "../../include/ast.h"
#include "../../include/hash.h"
#include "../../include/lex.h"
#include "../../include/lexeme.h"
#include "../../include/parse.h"
#include "inttypes.h"
#include "stdio.h"
#include "string.h"

void parse_exit(void *ptr) {
  if (ptr == NULL) {
    puts("[ERROR] | failure to allocate enough memory");
    puts("          in parser");
    exit(1);
  }
}

void check_size_parser_serial(parser_source_t *parser) {
  if (parser->cap <= parser->len) {
    parser->cap <<= 1;
    parser->asts = realloc(parser->asts, parser->cap * sizeof(ast_t *));
    parse_exit(parser->asts);
  }
}

void check_size_parser_free(parser_source_t *parser) {
  if (parser->free_cap <= parser->free_len) {
    parser->free_cap <<= 1;
    parser->ez_free =
        realloc(parser->ez_free, parser->free_cap * sizeof(ast_t *));
    parse_exit(parser->ez_free);
  }
}

parser_source_t parser_new() {
  parser_source_t val = {.len = 0,
                         .cap = 2,
                         .asts = NULL,
                         .ez_free = NULL,
                         .free_cap = 2,
                         .free_len = 0};
  val.asts = calloc(2, sizeof(ast_t *));
  val.ez_free = calloc(2, sizeof(ast_t *));
  parse_exit(val.asts);
  return val;
}

ast_t *parser_add_loose(parser_source_t *parser, ast_t ast) {
  check_size_parser_free(parser);
  ast_t *new_val = malloc(sizeof(ast_t));
  memcpy(new_val, &ast, sizeof(ast_t));
  memcpy(&parser->ez_free[parser->free_len++], &new_val, sizeof(ast_t *));
  return new_val;
}

void parser_add_serial(parser_source_t *parser, ast_t *ast) {
  check_size_parser_serial(parser);
  memcpy(&parser->asts[parser->len++], &ast, sizeof(ast_t *));
}

int has_token_consume(lex_source_t *lexer, token_e tok) {
  if (lex_peek(lexer).tok == tok) {
    lex_collect(lexer);
    return 1;
  }
  return 0;
}

token_e has_either_consume2(lex_source_t *lexer, token_e tok1, token_e tok2) {
  if (lex_peek(lexer).tok == tok1 || lex_peek(lexer).tok == tok2) {
    return lex_collect(lexer).tok;
  }
  return Empty;
}

ast_t *parse_ident(lex_source_t *lexer, parser_source_t *parser) {
  if (lex_peek(lexer).tok == Symbol) {
    lexeme_t val = lex_collect(lexer);
    char *symbol = calloc(val.span.len + 1, sizeof(char));
    memcpy(symbol, val.span.ptr, val.span.len);
    size_t hash = hash_it(symbol);
    ast_t ast = AST_Identifer(symbol, hash);
    return parser_add_loose(parser, ast);
  }
  return NULL;
}

ast_t *parse_true(lex_source_t *lexer, parser_source_t *parser) {
  if (has_token_consume(lexer, True)) {
    ast_t ast = AST_Single(True);
    return parser_add_loose(parser, ast);
  }
  return NULL;
}

ast_t *parse_false(lex_source_t *lexer, parser_source_t *parser) {
  if (has_token_consume(lexer, False)) {
    ast_t ast = AST_Single(False);
    return parser_add_loose(parser, ast);
  }
  return NULL;
}

ast_t *parse_null(lex_source_t *lexer, parser_source_t *parser) {
  if (has_token_consume(lexer, Null)) {
    ast_t ast = AST_Single(Null);
    return parser_add_loose(parser, ast);
  }
  return NULL;
}

ast_t *parse_num(lex_source_t *lexer, parser_source_t *parser) {
  if (is_num(lex_peek(lexer).tok)) {
    lexeme_t val = lex_collect(lexer);
    ast_t ast = AST_Num(atof(val.span.ptr));
    return parser_add_loose(parser, ast);
  }
  return NULL;
}

ast_t *parse_terminal(lex_source_t *lexer, parser_source_t *parser) {
  ast_t *val = NULL;
  val = parse_true(lexer, parser);
  if (val == NULL) {
    val = parse_false(lexer, parser);
    if (val == NULL) {
      val = parse_null(lexer, parser);
      if (val == NULL) {
        val = parse_num(lexer, parser);
        if (val == NULL) {
          val = parse_ident(lexer, parser);
        }
      }
    }
  }
  return val;
}

ast_t *parse_unary(lex_source_t *lexer, parser_source_t *parser) {
  token_e tok = has_either_consume2(lexer, Not, Sub);
  if (tok != Empty) {
    ast_t *unary = parse_unary(lexer, parser);
    ast_t ast = AST_Unary(tok, unary);
    return parser_add_loose(parser, ast);
  } else {
  }
  return NULL;
}

ast_t *parse_high_bin(lex_source_t *lexer, parser_source_t *parser) {
  ast_t *left = parse_terminal(lexer, parser);
  if (left != NULL) {
    while (is_high_bin(lex_peek(lexer).tok)) {
      token_e tok = lex_collect(lexer).tok;
      ast_t *right = parse_terminal(lexer, parser);
      if (right == NULL) {
        return right;
      }
      ast_t combined = AST_BinOp(left, tok, right);
      left = parser_add_loose(parser, combined);
    }
  }
  return left;
}

ast_t *parse_low_bin(lex_source_t *lexer, parser_source_t *parser) {
  ast_t *left = parse_high_bin(lexer, parser);
  if (left != NULL) {
    while (is_low_bin(lex_peek(lexer).tok)) {
      token_e tok = lex_collect(lexer).tok;
      ast_t *right = parse_high_bin(lexer, parser);
      if (right == NULL) {
        return right;
      }
      ast_t combined = AST_BinOp(left, tok, right);
      left = parser_add_loose(parser, combined);
    }
  }
  return left;
}

ast_t *parse_comp(lex_source_t *lexer, parser_source_t *parser) {
  ast_t *left = parse_low_bin(lexer, parser);
  if (left != NULL) {
    while (is_comp(lex_peek(lexer).tok)) {
      token_e tok = lex_collect(lexer).tok;
      ast_t *right = parse_low_bin(lexer, parser);
      if (right == NULL) {
        return right;
      }
      ast_t combined = AST_BinOp(left, tok, right);
      left = parser_add_loose(parser, combined);
    }
  }
  return left;
}

ast_t *parse_and_log(lex_source_t *lexer, parser_source_t *parser) {
  ast_t *left = parse_comp(lexer, parser);
  if (left != NULL) {
    while (lex_peek(lexer).tok == AndLog) {
      token_e tok = lex_collect(lexer).tok;
      ast_t *right = parse_comp(lexer, parser);
      if (right == NULL) {
        return right;
      }
      ast_t combined = AST_BinOp(left, tok, right);
      left = parser_add_loose(parser, combined);
    }
  }
  return left;
}

ast_t *parse_or_log(lex_source_t *lexer, parser_source_t *parser) {
  ast_t *left = parse_and_log(lexer, parser);
  if (left != NULL) {
    while (lex_peek(lexer).tok == OrLog) {
      token_e tok = lex_collect(lexer).tok;
      ast_t *right = parse_and_log(lexer, parser);
      if (right == NULL) {
        return right;
      }
      ast_t combined = AST_BinOp(left, tok, right);
      left = parser_add_loose(parser, combined);
    }
  }
  return left;
}

ast_t *parse_inner_assign(lex_source_t *lexer, parser_source_t *parser) {
  token_e has_token = has_either_consume2(lexer, Const, Mut);
  ast_t *ident = NULL;
  if (has_token != Empty) {
    ident = parse_ident(lexer, parser);
    if (ident != NULL) {
      if (has_token_consume(lexer, As)) {
        ast_t *or_log = parse_or_log(lexer, parser);
        if (or_log != NULL) {
          int has_semi = has_token_consume(lexer, SColon);
          ast_t combined = AST_Assign(ident, has_token, or_log, has_semi);
          ident = parser_add_loose(parser, combined);
        }
      }
    }
  }
  return ident;
}

ast_t *parse_reassign(lex_source_t *lexer, parser_source_t *parser) {
  ast_t *ident = parse_ident(lexer, parser);
  if (ident != NULL) {
    if (is_reassign(lex_peek(lexer).tok)) {
      token_e tok = lex_collect(lexer).tok;
      ast_t *or_log = parse_or_log(lexer, parser);
      if (or_log != NULL) {

        int has_semi = has_token_consume(lexer, SColon);
        ast_t combined = AST_Reassign(ident, tok, or_log, has_semi);
        ident = parser_add_loose(parser, combined);
      }
    }
  }
  return ident;
}

ast_t *parse_return(lex_source_t *lexer, parser_source_t *parser) {
  ast_t *comp = NULL;
  if (has_token_consume(lexer, Return)) {
    comp = parse_comp(lexer, parser);
    int has_semi = has_token_consume(lexer, SColon);
    ast_t combined = AST_Return(comp, has_semi);
    comp = parser_add_loose(parser, combined);
  }
  return comp;
}

ast_t *parse_expr(lex_source_t *lexer, parser_source_t *parser) {
  ast_t *inner_asgnmt = parse_inner_assign(lexer, parser);
  if (inner_asgnmt != NULL) {
    ast_t combined = AST_Expr(inner_asgnmt);
    inner_asgnmt = parser_add_loose(parser, combined);
  } else {
    inner_asgnmt = parse_reassign(lexer, parser);
    if (inner_asgnmt != NULL) {
      ast_t combined = AST_Expr(inner_asgnmt);
      inner_asgnmt = parser_add_loose(parser, combined);
    } else {
      inner_asgnmt = parse_return(lexer, parser);
      if (inner_asgnmt != NULL) {
        ast_t combined = AST_Expr(inner_asgnmt);
        inner_asgnmt = parser_add_loose(parser, combined);
      }
    }
  }
  return inner_asgnmt;
}

ast_t *parse_body(lex_source_t *lexer, parser_source_t *parser, int *start,
                  int *end) {
  int start_idx = parser->len;
  int end_idx = parser->len;
  if (!has_token_consume(lexer, OBrace)) {
    *start = -1;
    *end = -1;
    return NULL;
  }

  ast_t *expr = parse_expr(lexer, parser);

  while (expr != NULL) {
    parser_add_serial(parser, expr);
    end_idx++;
    expr = parse_expr(lexer, parser);
  }

  if (!has_token_consume(lexer, CBrace)) {
    *start = start_idx;
    *end = -1;
    return NULL;
  }
  ast_t body =
      AST_Body(NULL, parser->asts + start_idx, 0, (end_idx - start_idx));
  expr = parser_add_loose(parser, body);
  *start = start_idx;
  *end = end_idx - 1;
  return expr;
}

void parser_free(parser_source_t *parser) {
  for (size_t i = 0; i < parser->free_len; i++) {
    free(parser->ez_free[i]);
  }
  free(parser->asts);
  free(parser->ez_free);
}
