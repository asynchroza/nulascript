#include <ast.h>
#include <token.h>

std::string Program::tokenLiteral() {
    if (!statements.empty()) {
        // TODO: Look into using <stack>
        return statements.at(0).tokenLiteral();
    }

    return "";
}

std::string Identifier::tokenLiteral() { return token.literal; }
std::string LetStatement::tokenLiteral() { return token.literal; }
