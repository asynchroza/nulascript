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
    BANG_OR_NOT,
    SLASH,
    LT,
    LOE,
    GT,
    GOE,
    LET,
    IF,
    ELSE,
    TRUE,
    FALSE,
    RETURN,
    IS,
};

struct Token {
    TokenType type;
    std::string literal;
};

class TokenLookup {
public:
    TokenLookup();
    TokenType lookupIdent(const std::string& ident);

private:
    std::unordered_map<std::string, TokenType> keywords;
};

#endif
