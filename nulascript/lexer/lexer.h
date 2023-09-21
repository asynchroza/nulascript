#ifndef LEXER_H
#define LEXER_H

#include <string>
#include "token.h"

class Lexer {
public:
    Lexer(const std::string& input);
    Token getNextToken();

private:
    void readChar();
    std::string readExtendedToken(TokenType tokenType);
    void skipOverWhitespace();
    char peekNextChar();
    Token newToken(TokenType tokenType, char ch);
    bool isLetter(char ch);
    bool isDigit(char ch);

private:
    TokenLookup tokenLookup;
    std::string input;
    int pos;
    int readPos;
    char ch;
};

#endif // LEXER_H