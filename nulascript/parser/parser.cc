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
        std::string err =
            "[ERROR] No parsing function was found for prefix of type: " +
            std::to_string(currentToken.type);
        std::cout << err << std::endl;
        appendError(err);
        return nullptr;
    }

    // for clarity
    auto leftExpression = it->second();

    while (!isEqualToPeekedTokenType(TokenType::SEMICOLON) &&
           p < checkPeekPrecedence()) {
        auto infix_it = infixParsingFunctions.find(peekToken.type);

        if (infix_it == infixParsingFunctions.end()) {
            return leftExpression;
        }

        getNextToken();
        leftExpression = infix_it->second(leftExpression);
    }

    return leftExpression;
}

Precedence Parser::checkPeekPrecedence() {
    auto it = tokenPrecedences.find(peekToken.type);
    if (it == tokenPrecedences.end()) {
        return Precedence::LOWEST;
    }

    return tokenPrecedences.at(peekToken.type);
};

Precedence Parser::checkCurrentPrecedence() {
    auto it = tokenPrecedences.find(currentToken.type);
    if (it == tokenPrecedences.end()) {
        return Precedence::LOWEST;
    }

    return tokenPrecedences.at(currentToken.type);
};

Parser::Parser(Lexer& l) {
    this->l = &l;

    tokenPrecedences = {
        {TokenType::IS, Precedence::EQUALS},
        {TokenType::IS_NOT, Precedence::EQUALS},
        {TokenType::LT, Precedence::LESSGREATER},
        {TokenType::GT, Precedence::LESSGREATER},
        {TokenType::LOE, Precedence::LESSGREATER},
        {TokenType::GOE, Precedence::LESSGREATER},
        {TokenType::PLUS, Precedence::SUM},
        {TokenType::MINUS, Precedence::SUM},
        {TokenType::SLASH, Precedence::PRODUCT},
        {TokenType::ASTERISK, Precedence::PRODUCT},
    };

    getNextToken();
    getNextToken();

    registerPrefixFunction(TokenType::LPAR, [&]() -> Expression* {
        return parseParensExpressions();
    });
    registerPrefixFunction(TokenType::IDENT,
                           [&]() -> Expression* { return parseIdentifier(); });
    registerPrefixFunction(TokenType::TRUE,
                           [&]() -> Expression* { return parseBoolean(); });
    registerPrefixFunction(TokenType::FALSE,
                           [&]() -> Expression* { return parseBoolean(); });
    registerPrefixFunction(TokenType::INT,
                           [&]() -> Expression* { return parseInteger(); });
    registerPrefixFunction(TokenType::BANG_OR_NOT,
                           [&]() -> Expression* { return parsePrefix(); });
    registerPrefixFunction(TokenType::MINUS,
                           [&]() -> Expression* { return parsePrefix(); });
    registerInfixFunction(
        TokenType::MINUS,
        [&](Expression* left) -> Expression* { return parseInfix(left); });

    registerInfixFunction(
        TokenType::PLUS,
        [&](Expression* left) -> Expression* { return parseInfix(left); });

    registerInfixFunction(
        TokenType::SLASH,
        [&](Expression* left) -> Expression* { return parseInfix(left); });

    registerInfixFunction(
        TokenType::ASTERISK,
        [&](Expression* left) -> Expression* { return parseInfix(left); });

    registerInfixFunction(TokenType::IS, [&](Expression* left) -> Expression* {
        return parseInfix(left);
    });

    registerInfixFunction(
        TokenType::IS_NOT,
        [&](Expression* left) -> Expression* { return parseInfix(left); });

    registerInfixFunction(TokenType::LT, [&](Expression* left) -> Expression* {
        return parseInfix(left);
    });

    registerInfixFunction(TokenType::GT, [&](Expression* left) -> Expression* {
        return parseInfix(left);
    });

    registerInfixFunction(TokenType::GOE, [&](Expression* left) -> Expression* {
        return parseInfix(left);
    });

    registerInfixFunction(TokenType::LOE, [&](Expression* left) -> Expression* {
        return parseInfix(left);
    });
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

Expression* Parser::parseParensExpressions() {
    getNextToken();

    auto expression = parseExpression(Precedence::LOWEST);
    std::cout << expression->toString() << std::endl;

    if (!isEqualToPeekedTokenType(TokenType::RPAR)) {
        return nullptr;
    }

    return expression;
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
Boolean* Parser::parseBoolean() { return new Boolean(currentToken); }

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

// "!something" where ! is the Prefix expression and something is the right
// expression
Prefix* Parser::parsePrefix() {
    Prefix* expression = new Prefix(currentToken);
    getNextToken();
    expression->right = parseExpression(Precedence::PREFIX);

    return expression;
}

Infix* Parser::parseInfix(Expression* left) {
    Infix* expression = new Infix(currentToken, left);

    auto right = checkCurrentPrecedence();
    getNextToken();
    expression->right = parseExpression(right);

    return expression;
}
