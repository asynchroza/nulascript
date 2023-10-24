#include <ast.h>
#include <iostream>
#include <string>
#include <token.h>

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
        representation += statements[i]->toString() + "\n";
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
std::string Prefix::toString() { return op + right->toString(); }

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
