#include <parser.h>

void Parser::getNextToken() {
    currentToken = peekToken;
    peekToken = l->getNextToken();
}

Parser::Parser(Lexer *l) {
    this->l = l;

    getNextToken();
    getNextToken();
}

Program *Parser::parseProgram() { return nullptr; }