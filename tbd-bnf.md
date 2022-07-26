
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
