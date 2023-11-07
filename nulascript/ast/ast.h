#ifndef AST_H
#define AST_H

#include <string>
#include <token.h>
#include <vector>

class Node {
  public:
    virtual std::string tokenLiteral() = 0;
    virtual std::string toString() = 0;
};

class Statement : public Node {
  public:
    void statementNode();
};

class Expression : public Node {
  public:
    void expressionNode();

    // ! temp placeholders
    std::string tokenLiteral() override { return ""; }
    std::string toString() override { return ""; }
};

class Program : public Node {
  public:
    std::vector<Statement*> statements;
    std::string tokenLiteral() override;
    std::string toString() override;
};

// ? should expressionNode be virtual and overriden in child expressions?
class Identifier : public Expression {
  public:
    Token token;
    std::string value;

  public:
    Identifier(Token token);
    std::string tokenLiteral() override;
    std::string toString() override;
};

class Integer : public Expression {
  public:
    Token token;
    int64_t value;

  public:
    Integer(Token token);
    std::string tokenLiteral() override;
    std::string toString() override;
};

class Prefix : public Expression {
  public:
    Token token;
    std::string op;
    Expression* right;

  public:
    Prefix(Token token, Expression* expression);
    Prefix(Token token);
    std::string tokenLiteral() override;
    std::string toString() override;
};

class BlockStatement : public Statement {
  public:
    Token token;
    std::vector<Statement*> statements;

  public:
    BlockStatement(Token token);
    std::string toString();
    std::string tokenLiteral();
};

class Conditional : public Expression {
  public:
    Token token;
    Expression* condition;
    BlockStatement* currentBlock;
    BlockStatement* elseBlock;

  public:
    Conditional(Token token);
    std::string toString();
    std::string tokenLiteral();
};

class Infix : public Expression {
  public:
    Token token;
    Expression* left;
    Expression* right;
    std::string op;

  public:
    Infix(Token token, Expression* left, Expression* right);
    Infix(Token token, Expression* left);
    Infix(Token token);
    std::string tokenLiteral() override;
    std::string toString() override;
};

class Boolean : public Expression {
  public:
    Token token;
    bool value;

  public:
    Boolean(Token token);
    std::string tokenLiteral() override;
    std::string toString() override;
};

// ? should statementNode be virtual and overriden in child statements?
class LetStatement : public Statement {
  public:
    Token token;
    Identifier* name;
    Expression* value;

  public:
    LetStatement(Token token);
    std::string tokenLiteral() override;
    std::string toString() override;
};

class ReturnStatement : public Statement {
  public:
    Token token;
    Expression* returnValue;

  public:
    ReturnStatement(Token token, Expression* returnValue);
    // ! this should be eventually removed
    ReturnStatement(Token token);
    std::string tokenLiteral() override;
    std::string toString() override;
};

class ExpressionStatement : public Statement {
  public:
    Token token;
    Expression* expression;

  public:
    ExpressionStatement(Token token);
    // ExpressionStatement(Token token, Expression* expression)
    std::string tokenLiteral() override;
    std::string toString() override;
};

#endif

class Function : public Expression {
  public:
    Token token;
    std::vector<Identifier*> arguments;
    BlockStatement* code;

  public:
    Function(Token token);
    std::string tokenLiteral() override;
    std::string toString() override;
};