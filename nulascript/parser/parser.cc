#include <iostream>
#include <parser.h>
#include <token.h>

void Parser::getNextToken() {
    currentToken = peekToken;
    peekToken = l->getNextToken();
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

Expression* Parser::parseExpression(Precedence p) {
    auto it = prefixParsingFunctions.find(currentToken.type);

    if (it == prefixParsingFunctions.end()) {
        appendError("No parsing function was found for prefix of type: " +
                    std::to_string(currentToken.type));
        return nullptr;
    }

    // for clarity
    auto leftExpression = it->second();
    return leftExpression;
}

Parser::Parser(Lexer& l) {
    this->l = &l;

    getNextToken();
    getNextToken();

    registerPrefixFunction(TokenType::IDENT,
                           [&]() -> Expression* { return parseIdentifier(); });
    registerPrefixFunction(TokenType::INT,
                           [&]() -> Expression* { return parseInteger(); });
}

ExpressionStatement* Parser::parseExpressionStatement() {
    auto statement = new ExpressionStatement(currentToken);

    statement->expression = parseExpression(Precedence::LOWEST);
    if (isEqualToPeekedTokenType(TokenType::SEMICOLON)) {
        getNextToken();
    }

    return statement;
}

Statement* Parser::parseStatement() {
    switch (currentToken.type) {
    case TokenType::LET:
        return parseLetStatement();
    case TokenType::RETURN:
        return parseReturnStatement();
    default:
        return parseExpressionStatement();
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

Identifier* Parser::parseIdentifier() { return new Identifier(currentToken); }

Integer* Parser::parseInteger() {
    try {
        int64_t literal = stoi(currentToken.literal);
        auto lit = new Integer(currentToken);
        lit->value = literal;
        return lit;
    } catch (...) {
        appendError("Couldn't parse literal to integer");
        return nullptr;
    }
}
