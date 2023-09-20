#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <unordered_map>

enum TokenType {
    EOF_TYPE,
    ILLEGAL,
    IDENT,
    INT,
    ASSIGN,
    PLUS,
    MINUS,
    COMMA,
    SEMICOLON,
    LPAR,
    RPAR,
    LBRACE,
    RBRACE,
    ASTERISK,
    DEREF,
    PIPE,
    FUNC,
    BANG,
    SLASH,
    LT,
    GT,
    LET,
    IF,
    ELSE,
    TRUE,
    FALSE,
    RETURN,
    EQUAL,
    NOT_EQUAL,
};

struct Token {
    TokenType Type;
    std::string Literal;
};

class TokenLookup {
public:
    TokenLookup();
    TokenType LookupIdent(const std::string& ident);

private:
    std::unordered_map<std::string, TokenType> keywords;
};

#endif
