#include <ast.h>
#include <string>
#include <token.h>

// Program
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
LetStatement::LetStatement(Token token) : token(token) {}

std::string LetStatement::tokenLiteral() { return token.literal; }

std::string LetStatement::toString() {
    std::string representation = "";
    representation += tokenLiteral() + " " + name->toString() + " = ";

    if (value) {
        representation += value->toString();
    }

    representation += ";";
    return representation;
}

// Identifier
Identifier::Identifier(Token token) : token(token) {}

std::string Identifier::tokenLiteral() { return token.literal; }

std::string Identifier::toString() { return value; }

// ReturnStatement
ReturnStatement::ReturnStatement(Token token) : token(token) {}

ReturnStatement::ReturnStatement(Token token, Expression* returnValue)
    : token(token), returnValue(returnValue) {}

std::string ReturnStatement::tokenLiteral() { return token.literal; }

std::string ReturnStatement::toString() {
    std::string representation = "";

    representation += tokenLiteral() + " ";

    if (returnValue) {
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
