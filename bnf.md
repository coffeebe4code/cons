#bnf

working on
```
statement       => expression* | return ; // problem with implementing (call)
```
// return keyword is optional, which means the last expression could be a return, it could also be a call, and the result of that call is the return.
// ie func whatever(): num { getFive() }

implemented grammar 
```
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
