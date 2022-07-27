#bnf

implemented grammar 
```
body            => "{" statement+ "}" ;
statement       => expression | return ; // hollow
return          => "return"? or_log? ";"? ;
expression      => inner_asgnmt | reassignment | or_log;

inner_asgnmt    => ( "const" | "mut" ) IDENTIFIER ( "=" ) or_log ";"? ;
reassignment    => IDENTIFIER ( "=" | "/=" | "-=" | "+=" | "*=" | "&=" | "^=" | "|=" ) or_log ";"? ; // best place to do (call)
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
