#include <algorithm>
#include <ast.h>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <token.h>

bool to_bool(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    std::istringstream is(str);
    bool b;
    is >> std::boolalpha >> b;
    return b;
}

// Program
// ? should this implementation be dropped
std::string Program::tokenLiteral() {
    if (!statements.empty()) {
        return statements.at(0)->tokenLiteral();
    }

    return "";
}

std::string Program::toString() {
    std::string representation = "";
    for (int i = 0; i < statements.size(); i++) {
        representation += statements[i]->toString();
    }

    return representation;
}

// LetStatement
LetStatement::LetStatement(Token token)
    : token(token), name(nullptr), value(nullptr) {}

std::string LetStatement::tokenLiteral() { return token.literal; }

std::string LetStatement::toString() {
    std::string representation = "";
    representation += tokenLiteral() + " " + name->toString() + " = ";

    if (value) {
        std::cout << "[DEBUG] LetStatement Value is not NULL_PTR" << std::endl;
        representation += value->toString();
    }

    representation += ";";
    return representation;
}

// Identifier
// TODO: Value shouldn't be token.literal here
Identifier::Identifier(Token token) : token(token), value(token.literal) {}

std::string Identifier::tokenLiteral() { return token.literal; }

std::string Identifier::toString() { return value; }

// Integer
Integer::Integer(Token token) : token(token), value(stoi(token.literal)) {}

std::string Integer::tokenLiteral() { return token.literal; }
std::string Integer::toString() { return token.literal; }

// Prefix
Prefix::Prefix(Token token, Expression* expression)
    : token(token), right(expression), op(token.literal) {}
Prefix::Prefix(Token token) : token(token), op(token.literal) {}
std::string Prefix::tokenLiteral() { return token.literal; }
std::string Prefix::toString() { return "(" + op + right->toString() + ")"; }

// Infix
Infix::Infix(Token token, Expression* left, Expression* right)
    : token(token), left(left), right(right), op(token.literal){};
Infix::Infix(Token token, Expression* left)
    : token(token), left(left), op(token.literal){};
Infix::Infix(Token token) : token(token), op(token.literal){};
std::string Infix::tokenLiteral() { return token.literal; }
std::string Infix::toString() {
    return "(" + left->toString() + " " + op + " " + right->toString() + ")";
}

// Boolean
Boolean::Boolean(Token token) : token(token), value(to_bool(token.literal)){};
std::string Boolean::tokenLiteral() { return token.literal; }
std::string Boolean::toString() { return token.literal; }

// ReturnStatement
ReturnStatement::ReturnStatement(Token token)
    : token(token), returnValue(nullptr) {}

ReturnStatement::ReturnStatement(Token token, Expression* returnValue)
    : token(token), returnValue(returnValue) {}

std::string ReturnStatement::tokenLiteral() { return token.literal; }

std::string ReturnStatement::toString() {
    std::string representation = "";
    representation += tokenLiteral() + " ";

    if (returnValue) {
        std::cout << "[DEBUG] ReturnStatement returnValue is not NULL_PTR"
                  << std::endl;
        representation += returnValue->toString();
    }

    representation += ";";
    return representation;
}

// ExpressionStatement
ExpressionStatement::ExpressionStatement(Token token) : token(token){};

std::string ExpressionStatement::tokenLiteral() { return ""; }

std::string ExpressionStatement::toString() {
    if (expression) {
        return expression->toString();
    }

    return "";
}

Conditional::Conditional(Token token) : token(token) {}
std::string Conditional::toString() {
    std::string result =
        "if " + condition->toString() + " " + currentBlock->toString();
    if (elseBlock) {
        result += " else " + elseBlock->toString();
    }

    return result;
}

std::string Conditional::tokenLiteral() { return token.literal; }

BlockStatement::BlockStatement(Token token) : token(token) {}
std::string BlockStatement::tokenLiteral() { return token.literal; }

std::string BlockStatement::toString() {
    std::string result = "";
    for (auto stmt : statements) {
        result += stmt->toString();
    }

    return result;
}

Function::Function(Token token) : token(token) {}
std::string Function::toString() {
    std::string result = "";
    result += token.literal + "(";
    for (auto param : arguments) {
        result += param->tokenLiteral() + ", ";
    }
    result += ")";

    return result;
}
std::string Function::tokenLiteral() { return token.literal; }