#bnf

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

```
or_log          => or_log ( "||" and_log )* ;
and_log         => equality ( "&&" equality )* ;
equality        => comp ( ( "!=" | "==" ) comp )* ;
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
function        => "pub"? IDENTIFIER "(" parameters? ")" (":" VAL_TYPE | IDENTIFIER )? block ;
block           => "{" statement+ "}" ;


```

implemented grammer  
```

statement       => expression* | return ; 
return          => "return" low_bin ";"? ;
expression      => inner_asgnmt | reassignment; 

inner_asgnmt      => ( "const" | "mut" ) IDENTIFIER ( "=" ) low_bin ";"? ;
reassignment    => IDENTIFIER ( "=" | "/=" | "-=" | "+=" | "*=" | "&=" | "^=" | "|=" ) low_bin ";"? ;
comp            => low_bin ( ( ">" | ">=" | "<" | "<=" ) low_bin )* ;
low_bin         => high_bin ( ( "-" | "+") high_bin )* ;
high_bin        => TERMINAL ( ( "/" | "*" | "%" ) TERMINAL )* ;

TERMINAL        => NUMBER | IDENTIFIER ;
NUMBER          => DIGIT+ ( "." DIGIT+ )? ;
IDENTIFIER      => < any ALPHA except "_" > ( ALPHA | DIGIT )* ;
```
