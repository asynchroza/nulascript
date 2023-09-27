#include <iostream>
#include <parser.h>
#include <token.h>

void Parser::getNextToken() {
    currentToken = peekToken;
    peekToken = l->getNextToken();
}

Parser::Parser(Lexer& l) {
    this->l = &l;

    getNextToken();
    getNextToken();
}

bool Parser::isEqualToCurrentTokenType(TokenType tokenType) {
    return currentToken.type == tokenType;
}

bool Parser::isEqualToPeekedTokenType(TokenType tokenType) {
    return peekToken.type == tokenType;
}

/*
    Compares the type of the peeked token to the passed type.
    If they match, parser loads peeked token as current token.
*/
bool Parser::peekAndLoadExpectedToken(TokenType tokenType) {
    if (isEqualToPeekedTokenType(tokenType)) {
        getNextToken();
        return true;
    }

    return false;
}

LetStatement* Parser::parseLetStatement() {
    LetStatement* letStatement = new LetStatement(currentToken);

    if (!peekAndLoadExpectedToken(TokenType::IDENT)) {
        return nullptr;
    }

    letStatement->name = new Identifier(currentToken);

    if (!peekAndLoadExpectedToken(TokenType::ASSIGN)) {
        return nullptr;
    }

    // TODO: Skip expressions
    while (!isEqualToCurrentTokenType(TokenType::SEMICOLON)) {
        getNextToken();
    }

    return letStatement;
}

Statement* Parser::parseStatement() {
    switch (currentToken.type) {
    case TokenType::LET:
        return parseLetStatement();
    default:
        return nullptr;
    }
}

Program* Parser::parseProgram() {
    Program* program = new Program();

    while (currentToken.type != TokenType::EOF_TYPE) {
        Statement* statement = this->parseStatement();

        if (statement) {
            program->statements.push_back(statement);
        }

        getNextToken();
    }

    return program;
}
