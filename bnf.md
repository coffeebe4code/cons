#bnf

```
start           => declaration* EOF ;
declaration     => type_decl | const_decl | mut_decl ;
type_decl       => "pub"? "type" IDENTIFIER body_decl ;
const_decl      => "pub"? "const" assignment ;
mut_decl        => "pub"? "mut" assignment ;
repr_decl       => "pub"? "repr" IDENTIFIER body_decl ;
body_decl       => "{" properties+ "}" ";"? ;
properties      => ("pub"? IDENTIFIER ":" (IDENTIFIER | VAL_TYPE) ";"?) ;


```

```
NUMBER          => DIGIT+ ( "." DIGIT+ )? ;
UTF8_STRING     => '"' < any CHAR except '"' | escaped char list except '\'' >* '"' ;
IDENTIFIER      => < any ALPHA except "_" > ( ALPHA | DIGIT )* ;
ALPHA           => "a" ... "z" | "A" ... "Z" | "_" ;
DIGIT           => "0" ... "9" ;
CHAR            => "'" <any character except "'" | escaped char list> "'" ;
TERMINAL        => "true" | "false" | "this" | "null" | NUMBER | UTF8_STRING | IDENTIFIER ;
VAL_TYPE        => <value types in ./include/token.h>
```

precedence
```
or_log          => or_log ( "||" and_log )* ;
and_log         => equality ( "&&" equality )* ;
equality        => comp ( ( "!=" | "==" ) comp )* ;
comp            => low_bin ( ( ">" | ">=" | "<" | "<=" ) low_bin )* ;
low_bin         => high_bin ( ( "-" | "+" | "%" ) high_bin )* ;
high_bin        => unary ( ( "/" | "*" ) unary )* ;
unary           => ( "!" | "-" ) unary | invoke ;
```

```
declaration     => type_decl | const_decl | mut_decl ;
type_decl       => "pub"? "type" IDENTIFIER body_decl ;
const_decl      => "pub"? "const" assignment ;
mut_decl        => "pub"? "mut" assignment ;
body_decl       => "{" properties+ "}" ";"? ;
properties      => ("pub"? IDENTIFIER ":" (IDENTIFIER | VAL_TYPE) ";"?) properties;

root_assignment => "pub"? assignment ;
assignment      => ("const" | "mut") IDENTIFIER ( "=" | "/=" | "+=" | "*=" | "&=" | "^=" | "|=" ) expression ;
function        => "pub"? IDENTIFIER "(" parameters? ")" (":" VAL_TYPE | IDENTIFIER )? block ;
block           => "{" statement+ "}" ;


```

implement grammar    
```
statement       => expression* | return ; // initially only expressions and a final return is supported
return          => "return" low_bin ";"? ;
expression      => assignment | reassignment; 

assignment      => ("const" | "mut") IDENTIFIER ( "=" ) low_bin ";"? ;
reassignment    => IDENTIFIER ( "=" | "/=" | "-=" | "+=" | "*=" | "&=" | "^=" | "|=" ) low_bin ";"? ;
low_bin         => high_bin ( ( "-" | "+") high_bin )* ;
high_bin        => TERMINAL ( ( "/" | "*" | "%" ) TERMINAL )* ;

TERMINAL        => NUMBER | IDENTIFIER ;
NUMBER          => DIGIT+ ( "." DIGIT+ )? ;
IDENTIFIER      => < any ALPHA except "_" > ( ALPHA | DIGIT )* ;
```
