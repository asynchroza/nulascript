#ifndef PARSER_H
#define PARSER_H

#include <ast.h>
#include <lexer.h>
#include <token.h>

class Parser {
  public:
    Lexer* l;

    Token currentToken;
    Token peekToken;

  public:
    Parser(Lexer& l);
    void getNextToken();

    Program* parseProgram();
    Statement* parseStatement();
    LetStatement* parseLetStatement();
    bool isEqualToCurrentTokenType(TokenType tokenType);
    bool isEqualToPeekedTokenType(TokenType tokenType);
    bool peekAndLoadExpectedToken(TokenType tokenType);
};

#endif
