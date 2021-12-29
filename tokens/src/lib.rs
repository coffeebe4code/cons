use std::num::ParseIntError;

use logos::{Logos};

#[derive(Logos, Debug, PartialEq)]
enum Token {
    #[token("static")]
    Static,
    #[token("inline")]
    Inline,
    #[token("macro")]
    Macro,

    #[token("(")]
    OParen,
    #[token(")")]
    CParen,
    #[token("[")]
    OArray,
    #[token("]")]
    CArray,

    #[token(".")]
    Period,

    #[regex("[a-zA-Z]+")]
    Symbol,
    #[regex("0x[0-9A-F]+")]
    Hex,
    #[regex("0b[0-1]+")]
    Bin,
    #[regex("[0-9]+")]
    Num,

    #[error]
    #[regex(r"[ \t\r\n\f]+", logos::skip)]
    Error
    
}
#[cfg(test)]
mod tests {
    use crate::*;
    #[test]
    fn lex_keywords() {
        let mut lexer = Token::lexer("static inline macro");
        assert_eq!(lexer.next(), Some(Token::Static));
        assert_eq!(lexer.span(), 0..6);
        assert_eq!(lexer.next(), Some(Token::Inline));
        assert_eq!(lexer.span(), 7..13);
        assert_eq!(lexer.next(), Some(Token::Macro));
        assert_eq!(lexer.span(), 14..19);
    }
    #[test]
    fn lex_numbers() {
        let mut lexer = Token::lexer("222 0x22FF 0b01011");
        assert_eq!(lexer.next(), Some(Token::Num));
        assert_eq!(lexer.span(), 0..3);
        assert_eq!(lexer.next(), Some(Token::Hex));
        assert_eq!(lexer.span(), 4..10);
        assert_eq!(lexer.next(), Some(Token::Bin));
        assert_eq!(lexer.span(), 11..18);
    }
}
