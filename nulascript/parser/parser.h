#ifndef PARSER_H
#define PARSER_H

#include <ast.h>
#include <functional>
#include <lexer.h>
#include <map>
#include <token.h>

enum class Precedence {
    LOWEST,
    EQUALS,
    LESSGREATER,
    SUM,
    PRODUCT,
    PREFIX,
    CALL
};

// ! this is not explained in thesis
// ? should this be Expression or ExpressionStatement
using ParsePrefixFunction = std::function<Expression*()>;
using ParseInfixFunction = std::function<Expression*(Expression*)>;

class Parser {
  private:
    Lexer* l;

    Token currentToken;
    Token peekToken;

    std::map<TokenType, Precedence> tokenPrecedences;

    std::vector<std::string> errors;
    std::map<TokenType, ParsePrefixFunction> prefixParsingFunctions;
    std::map<TokenType, ParseInfixFunction> infixParsingFunctions;

  public:
    Parser(Lexer& l);
    void getNextToken();

    Program* parseProgram();
    Statement* parseStatement();
    Expression* parseExpression(Precedence p);
    LetStatement* parseLetStatement();
    ReturnStatement* parseReturnStatement();
    ExpressionStatement* parseExpressionStatement();
    Integer* parseInteger();
    Expression* parseIdentifier();
    Prefix* parsePrefix();
    Boolean* parseBoolean();
    Infix* parseInfix(Expression* left);
    Conditional* parseConditional();
    BlockStatement* parseBlock();
    Expression* parseParensExpressions();
    Function* parseFunction();
    String* parseString();
    Expression* parseInvocation(Expression* function);
    std::vector<Expression*> parseInvocationArguments();
    std::vector<Identifier*> parseFunctionArguments();
    bool isEqualToCurrentTokenType(TokenType tokenType);
    bool isEqualToPeekedTokenType(TokenType tokenType);
    bool peekAndLoadExpectedToken(TokenType tokenType);
    void registerPrefixFunction(TokenType tokenType,
                                ParsePrefixFunction prefixParsingFunction);
    void registerInfixFunction(TokenType tokenType,
                               ParseInfixFunction infixParsingFunction);
    Precedence checkCurrentPrecedence();
    Precedence checkPeekPrecedence();

    std::vector<std::string> getErrors();
    void appendError(std::string e);
    void appendPeekError(TokenType token);
};

#endif
