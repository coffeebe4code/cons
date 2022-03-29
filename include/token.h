#pragma once
#include "./span.h"
#define TOKEN_LENGTH 46

// clang-format off
static char *keyword_list[TOKEN_LENGTH] __attribute__((unused)) = {
  "import", 
  "define",
  "macro",
  "test",
  "mut",
  "const",
  "i32",
  "u32",
  "u64",
  "i64",
  "i16",
  "u16",
  "u8",
  "i8",
  "bit",
  "f64",
  "f32",
  "d32",
  "d64",
  "if",
  "else",
  "type",
  "this",
  "null",
  "char",
  "string",
  "inline",
  "static",
  "switch",
  "for",
  "in",
  "break",
  "enum",
  "pub",
  "return",
  "async",
  "await",
  "box",
  "trait",
  "ptr",
  "match",
  "addr",
  "vol",
  "true",
  "false",
  "void"
};

static int keyword_len[TOKEN_LENGTH] __attribute__((unused)) = {
  6,
  6,
  5,
  4,
  3, // mut
  5,
  3,
  3,
  3,
  3,
  3,
  3,
  2, // u8
  2,
  3,
  3,
  3, // f32
  3,
  3,
  2,
  4, // else
  4,
  4,
  4,
  4,
  6, // string
  6,
  6,
  6,
  3, // for
  2,
  5,
  4,
  3,
  6, // return
  5,
  5,
  3,
  5,
  3,
  5,
  4,
  3,
  4,
  5,
  4,
};
// clang-format on

typedef enum token_e {
  Import,
  Define,
  Macro,
  Test,
  Mut,
  Const,
  I32,
  U32,
  U64,
  I64,
  I16,
  U16,
  U8,
  I8,
  Bit,
  F64,
  F32,
  D32,
  D64,
  If,
  Else,
  Type,
  This,
  Null,
  Char,
  WString,
  Inline,
  Static,
  Switch,
  For,
  In,
  Break,
  Enum,
  Pub,
  Return,
  Async,
  Await,
  WBox,
  Trait,
  Ptr,
  Match,
  Addr,
  Vol,
  True,
  False,
  Void,
  OParen,
  CParen,
  OBrace,
  CBrace,
  OArray,
  CArray,
  Dot,
  Comma,
  Dollar,
  Question,
  Pound,
  Colon,
  SColon,
  Backtick,
  At,
  Lt,
  Gt,
  Div,
  BSlash,
  Plus,
  Rest,
  Sub,
  Mul,
  Or,
  And,
  Xor,
  LShift,
  RShift,
  Not,
  As,
  NotAs,
  OrAs,
  XorAs,
  LShiftAs,
  RShiftAs,
  AndLog,
  OrgLog,
  NotEquality,
  Equality,
  NotLog,
  Mod,
  Inc,
  Dec,
  AddAs,
  SubAs,
  DivAs,
  ModAs,
  DQuote,
  SQuote,
  Symbol,
  Hex,
  Bin,
  Num,
  NewLine,
  Error,
} token_e;

typedef struct {
  token_e token;
  span_t span;
} token_t;

token_t token_next(char *data);
char *token_error(token_t *token);
