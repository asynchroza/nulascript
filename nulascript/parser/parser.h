#ifndef PARSER_H
#define PARSER_H

#include <ast.h>
#include <lexer.h>
#include <token.h>

class Parser {
  private:
    Lexer* l;

    Token currentToken;
    Token peekToken;

    std::vector<std::string> errors;

  public:
    Parser(Lexer& l);
    void getNextToken();

    Program* parseProgram();
    Statement* parseStatement();
    LetStatement* parseLetStatement();
    ReturnStatement* parseReturnStatement();
    bool isEqualToCurrentTokenType(TokenType tokenType);
    bool isEqualToPeekedTokenType(TokenType tokenType);
    bool peekAndLoadExpectedToken(TokenType tokenType);

    std::vector<std::string> getErrors();
    void appendError(std::string e);
    void appendPeekError(TokenType token);

  private:
};

#endif
