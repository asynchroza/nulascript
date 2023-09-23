#ifndef AST_H
#define AST_H

#include <string>
#include <token.h>
#include <vector>

class Node {
  public:
    virtual std::string tokenLiteral() = 0;
};

class Statement : public Node {
  public:
    void statementNode();
};

class Expression : public Node {
  public:
    void expressionNode();
};

class Program : public Node {
  public:
    std::vector<Statement> statements;
    std::string tokenLiteral() override;
};

// ? should expressionNode be virtual and overriden in child expressions?
class Identifier : public Expression {
  public:
    Token token;
    std::string value;

  public:
    std::string tokenLiteral() override;
};

// ? should statementNode be virtual and overriden in child statements?
class LetStatement : public Statement {
  public:
    Token token;
    Identifier *name;
    Expression *value;

  public:
    std::string tokenLiteral() override;
};

#endif