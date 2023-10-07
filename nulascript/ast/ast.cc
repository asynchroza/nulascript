#include <ast.h>
#include <string>
#include <token.h>

std::string Program::tokenLiteral() {
    if (!statements.empty()) {
        return statements.at(0)->tokenLiteral();
    }

    return "";
}

LetStatement::LetStatement(Token token) : token(token) {}
Identifier::Identifier(Token token) : token(token) {}
ReturnStatement::ReturnStatement(Token token, Expression* returnValue)
    : token(token), returnValue(returnValue) {}

std::string Identifier::tokenLiteral() { return token.literal; }
std::string LetStatement::tokenLiteral() { return token.literal; }
std::string ReturnStatement::tokenLiteral() { return token.literal; }
