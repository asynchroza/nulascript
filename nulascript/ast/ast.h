#pragma once
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

class Statement : public Node {};

class Expression : public Node {
  public:
    std::string tokenLiteral() override { return ""; }
    std::string toString() override { return ""; }
};

class Program : public Node {
  public:
    std::vector<Statement*> statements;
    std::string tokenLiteral() override;
    std::string toString() override;
};

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
    bool hasCode();
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

class Invocation : public Expression {
  public:
    Token token;
    Function* function;
    std::vector<Expression*> arguments;

  public:
    Invocation(Token token, Function* function);
    std::string tokenLiteral() override;
    std::string toString() override;
};

struct ForLoopInitialization {
    LetStatement* variable;
    Infix* conditional;
    Infix* increment;
};

class ForLoop : public Expression {
  public:
    Token token;
    BlockStatement* code;
    // loop variables initialization
    ForLoopInitialization definition;

  public:
    ForLoop(Token token);
    std::string tokenLiteral() override;
    std::string toString() override;
};

class String : public Expression {
  public:
    Token token;
    std::string value;

  public:
    String(Token token);
    std::string tokenLiteral() override;
    std::string toString() override;
};

class Assignment : public Expression {
  public:
    Token token;
    Identifier* identifier;
    Expression* expression;

  public:
    Assignment(Token token, Identifier* identifier);
    std::string tokenLiteral() override;
    std::string toString() override;
};

class Reference : public Expression {
  public:
    Token token;
    std::string referencedIdentifier;

  public:
    Reference(Token token);
    std::string tokenLiteral() override;
    std::string toString() override;
};

class Pointer : public Expression {
  public:
    Token token;
    std::string dereferencedIdentifier;

  public:
    Pointer(Token token);
    std::string tokenLiteral() override;
    std::string toString() override;
};

class Comment : public Expression {
  public:
    Token token;

  public:
    Comment(Token token);
    std::string tokenLiteral() override;
    std::string toString() override;
};