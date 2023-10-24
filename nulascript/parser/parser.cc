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

std::vector<std::string> Parser::getErrors() { return errors; }

void Parser::appendError(std::string e) { errors.push_back(e); }

void Parser::appendPeekError(TokenType tokenType) {
    std::string error = "Expected token::type " + std::to_string(tokenType) +
                        "; Got token::type " + std::to_string(peekToken.type);
    appendError(error);
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

    appendPeekError(tokenType);
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

    // TODO: -> It's skipping all expressions now
    while (!isEqualToCurrentTokenType(TokenType::SEMICOLON)) {
        getNextToken();
    }

    return letStatement;
}

ReturnStatement* Parser::parseReturnStatement() {
    ReturnStatement* returnStatement = new ReturnStatement(currentToken);

    getNextToken();

    // TODO: -> It's skipping all expressions now
    while (!isEqualToCurrentTokenType(TokenType::SEMICOLON)) {
        getNextToken();
    }

    return returnStatement;
}

Statement* Parser::parseStatement() {
    switch (currentToken.type) {
    case TokenType::LET:
        return parseLetStatement();
    case TokenType::RETURN:
        return parseReturnStatement();
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

// ! not explained in thesis
void Parser::registerPrefixFunction(TokenType tokenType,
                                    ParsePrefixFunction parsePrefixFunction) {
    prefixParsingFunctions[tokenType] = parsePrefixFunction;
}

void Parser::registerInfixFunction(TokenType tokenType,
                                   ParseInfixFunction parseInfixFunction) {
    infixParsingFunctions[tokenType] = parseInfixFunction;
}