# EBNF of cons

```
program         => ( top )* EOF

signature       => VAL_TYPE | serial_types
serial_types    => IDENTIFIER | ( IDENTIFIER "," serial_types )

top             => "pub"? ( func_decl | body | expression | type_decl )
func_decl       => "fn" IDENTIFIER "(" arguments* ")" body
type_decl       => "type" IDENTIFIER (":" signature)? properties
properties      => "{" property+ "}"
property        => "pub"? ( ( IDENTIFIER ":" signature ( "," | ";" )? ) | func_decl )
body            => "{" expression+ "}"
expression      => inner_asgnmt | reassignment | return
return          => "return" or_log? ";"?

inner_asgnmt    => ( "const" | "mut" ) IDENTIFIER ( ":" signature )? ( "=" ) or_log ";"?
reassignment    => IDENTIFIER ( "=" | "/=" | "-=" | "+=" | "*=" | "&=" | "^=" | "|=" ) or_log ";"?
or_log          => and_log ( "||" and_log )* 
and_log         => equality ( "&&" equality )*
equality        => comp ( ( "!=" | "==" ) comp )*
comp            => low_bin ( ( ">" | ">=" | "<" | "<=" ) low_bin )*
low_bin         => high_bin ( ( "-" | "+") high_bin )*
high_bin        => unary ( ( "/" | "*" | "%" ) unary )*
unary           => ( "!" | "-" ) unary

TERMINAL        => "true" | "false" | "null" | NUMBER | IDENTIFIER
NUMBER          => DIGIT+ ( "." DIGIT+ )?
IDENTIFIER      => < any ALPHA except "_" > ( ALPHA | DIGIT )*
```
