#bnf

```
UTF8_STRING     => '"' < any CHAR except '"' | escaped char list except '\'' >* '"' ;
CHAR            => "'" <any character except "'" | escaped char list> "'" ;
VAL_TYPE        => <value types in ./include/token.h>
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
block           => "{" statement* "}" ;


```

implemented grammer  
```
statement       => expression* | return ; 
return          => "return"? or_log? ";"? ;
expression      => inner_asgnmt | reassignment; 

inner_asgnmt    => ( "const" | "mut" ) IDENTIFIER ( "=" ) low_bin ";"? ;
reassignment    => IDENTIFIER ( "=" | "/=" | "-=" | "+=" | "*=" | "&=" | "^=" | "|=" ) low_bin ";"? ;
or_log          => and_log ( "||" and_log )* ;
and_log         => equality ( "&&" equality )* ;
equality        => comp ( ( "!=" | "==" ) comp )* ;
comp            => low_bin ( ( ">" | ">=" | "<" | "<=" ) low_bin )* ;
low_bin         => high_bin ( ( "-" | "+") high_bin )* ;
high_bin        => unary ( ( "/" | "*" | "%" ) unary )* ;
unary           => ( "!" | "-" ) unary;

TERMINAL        => "true" | "false" | "null" | NUMBER | IDENTIFIER ;
NUMBER          => DIGIT+ ( "." DIGIT+ )? ;
IDENTIFIER      => < any ALPHA except "_" > ( ALPHA | DIGIT )* ;
```
