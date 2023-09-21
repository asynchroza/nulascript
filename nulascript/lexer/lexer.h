#ifndef LEXER_H
#define LEXER_H

#include <string>
#include "token.h"

class Lexer {
public:
    Lexer(const std::string& input);
    Token GetNextToken();

private:
    void ReadChar();
    std::string ReadExtendedToken(TokenType tokenType);
    void SkipOverWhitespace();
    char PeekNextChar();
    Token NewToken(TokenType tokenType, char ch);
    bool IsLetter(char ch);
    bool IsDigit(char ch);

private:
    std::string input;
    int pos;
    int readPos;
    char ch;
};

#endif // LEXER_H