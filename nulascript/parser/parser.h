#ifndef PARSER_H
#define PARSER_H

#include <ast.h>
#include <functional>
#include <lexer.h>
#include <map>
#include <token.h>

// ! this is not explained in thesis
// ? should this be Expression or ExpressionStatement
using ParsePrefixFunction = std::function<ExpressionStatement()>;
using ParseInfixFunction =
    std::function<ExpressionStatement(ExpressionStatement)>;

class Parser {
  private:
    Lexer* l;

    Token currentToken;
    Token peekToken;

    std::vector<std::string> errors;
    std::map<TokenType, ParsePrefixFunction> prefixParsingFunctions;
    std::map<TokenType, ParseInfixFunction> infixParsingFunctions;

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
    void registerPrefixFunction(TokenType tokenType,
                                ParsePrefixFunction prefixParsingFunction);
    void registerInfixFunction(TokenType tokenType,
                               ParseInfixFunction infixParsingFunction);

    std::vector<std::string> getErrors();
    void appendError(std::string e);
    void appendPeekError(TokenType token);

  private:
};

#endif
