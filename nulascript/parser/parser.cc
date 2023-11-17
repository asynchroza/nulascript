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

    getNextToken();

    letStatement->value = parseExpression(Precedence::LOWEST);

    if (isEqualToPeekedTokenType(TokenType::SEMICOLON)) {
        getNextToken();
    }

    return letStatement;
}

ReturnStatement* Parser::parseReturnStatement() {
    ReturnStatement* returnStatement = new ReturnStatement(currentToken);

    getNextToken();

    returnStatement->returnValue = parseExpression(Precedence::LOWEST);

    if (isEqualToPeekedTokenType(TokenType::SEMICOLON)) {
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

    tokenPrecedences = {{TokenType::IS, Precedence::EQUALS},
                        {TokenType::IS_NOT, Precedence::EQUALS},
                        {TokenType::LT, Precedence::LESSGREATER},
                        {TokenType::GT, Precedence::LESSGREATER},
                        {TokenType::LOE, Precedence::LESSGREATER},
                        {TokenType::GOE, Precedence::LESSGREATER},
                        {TokenType::PLUS, Precedence::SUM},
                        {TokenType::MINUS, Precedence::SUM},
                        {TokenType::SLASH, Precedence::PRODUCT},
                        {TokenType::ASTERISK, Precedence::PRODUCT},
                        {TokenType::LPAR, Precedence::CALL}};

    getNextToken();
    getNextToken();

    registerPrefixFunction(TokenType::LPAR, [&]() -> Expression* {
        return parseParensExpressions();
    });
    registerPrefixFunction(TokenType::IDENT,
                           [&]() -> Expression* { return parseIdentifier(); });
    registerPrefixFunction(TokenType::REF,
                           [&]() -> Expression* { return parseReference(); });
    registerPrefixFunction(TokenType::STRING,
                           [&]() -> Expression* { return parseString(); });
    registerPrefixFunction(TokenType::FUNC,
                           [&]() -> Expression* { return parseFunction(); });
    registerPrefixFunction(TokenType::IF,
                           [&]() -> Expression* { return parseConditional(); });
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
    registerPrefixFunction(TokenType::ASTERISK,
                           [&]() -> Expression* { return parsePrefix(); });
    registerPrefixFunction(
        TokenType::COLON, [&]() -> Expression* { return parseModuleAccess(); });
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
        TokenType::LPAR,
        [&](Expression* left) -> Expression* { return parseInvocation(left); });

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

    if (!peekAndLoadExpectedToken(TokenType::RPAR)) {
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

Expression* Parser::parseIdentifier() {
    auto currentIdentifier = new Identifier(currentToken);
    if (isEqualToPeekedTokenType(TokenType::ASSIGN)) {
        // make sure it's two times and not one
        getNextToken();
        getNextToken();
        auto assignment = new Assignment(currentToken, currentIdentifier);
        assignment->expression = parseExpression(Precedence::LOWEST);
        return assignment;
    }

    return new Identifier(currentToken);
}
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

BlockStatement* Parser::parseBlock() {
    auto currentBlock = new BlockStatement(currentToken);
    currentBlock->statements = std::vector<Statement*>(); // ?

    getNextToken();

    while (!isEqualToCurrentTokenType(TokenType::EOF_TYPE) &&
           !isEqualToCurrentTokenType(TokenType::RBRACE)) {
        auto statement = parseStatement();
        if (statement)
            currentBlock->statements.push_back(statement);
        getNextToken();
    }

    return currentBlock;
}

Conditional* Parser::parseConditional() {
    auto conditional = new Conditional(currentToken);

    if (!peekAndLoadExpectedToken(TokenType::LPAR)) {
        return nullptr;
    }

    getNextToken();
    conditional->condition = parseExpression(Precedence::LOWEST);

    if (!peekAndLoadExpectedToken(TokenType::RPAR)) {
        return nullptr;
    }

    if (!peekAndLoadExpectedToken(TokenType::LBRACE)) {
        return nullptr;
    }

    conditional->currentBlock = parseBlock();

    if (isEqualToPeekedTokenType(TokenType::ELSE)) {
        getNextToken();

        if (!peekAndLoadExpectedToken(TokenType::LBRACE)) {
            return nullptr;
        }

        conditional->elseBlock = parseBlock();
    }

    return conditional;
}

std::vector<Expression*> Parser::parseInvocationArguments() {
    std::vector<Expression*> arguments = std::vector<Expression*>();

    if (isEqualToPeekedTokenType(TokenType::RPAR)) {
        getNextToken();
        return arguments;
    }

    getNextToken();
    arguments.push_back(parseExpression(Precedence::LOWEST));

    while (isEqualToPeekedTokenType(TokenType::COMMA)) {
        getNextToken();
        getNextToken();
        arguments.push_back(parseExpression(Precedence::LOWEST));
    }

    if (!peekAndLoadExpectedToken(TokenType::RPAR)) {
        return std::vector<Expression*>();
    }

    return arguments;
}

std::vector<Identifier*> Parser::parseFunctionArguments() {
    auto arguments = std::vector<Identifier*>();

    if (isEqualToPeekedTokenType(TokenType::RPAR)) {
        getNextToken();
        return arguments;
    }

    getNextToken();
    auto identifier = new Identifier(currentToken);
    arguments.push_back(identifier);

    while (isEqualToPeekedTokenType(TokenType::COMMA)) {
        getNextToken();
        getNextToken();
        identifier = new Identifier(currentToken);
        arguments.push_back(identifier);
    }

    if (!peekAndLoadExpectedToken(TokenType::RPAR)) {
        return std::vector<Identifier*>();
    }

    return arguments;
}

Function* Parser::parseFunction() {
    auto func = new Function(currentToken);

    if (!peekAndLoadExpectedToken(TokenType::LPAR)) {
        return nullptr;
    }

    func->arguments = parseFunctionArguments();

    if (!peekAndLoadExpectedToken(TokenType::LBRACE)) {
        return nullptr;
    }

    func->code = parseBlock();
    return func;
}

Expression* Parser::parseInvocation(Expression* function) {
    auto invocation = new Invocation(currentToken, (Function*)function);
    invocation->arguments = parseInvocationArguments();
    return invocation;
}

String* Parser::parseString() { return new String(currentToken); }

Reference* Parser::parseReference() {
    auto ref = new Reference(currentToken);

    if (!peekAndLoadExpectedToken(TokenType::IDENT)) {
        return nullptr;
    }

    ref->referencedIdentifier = currentToken.literal;
    getNextToken();

    return ref;
}

Expression* Parser::parseModuleAccess() {
    Token moduleAccessToken = currentToken;

    if (!peekAndLoadExpectedToken(TokenType::IDENT)) {
        return nullptr;
    }

    std::string lib = currentToken.literal;

    if (!peekAndLoadExpectedToken(TokenType::MINUS) ||
        !peekAndLoadExpectedToken(TokenType::GT) ||
        !peekAndLoadExpectedToken(TokenType::IDENT)) {
        return nullptr;
    }

    std::string member = currentToken.literal;

    // this means it's a function invocation
    if (isEqualToPeekedTokenType(TokenType::LPAR)) {
        getNextToken();
        auto moduleInvocation = new ModuleInvocation(moduleAccessToken);
        moduleInvocation->module = lib;
        moduleInvocation->member = member;
        moduleInvocation->arguments = parseInvocationArguments();
        return moduleInvocation;
    }

    return nullptr;
    // auto moduleMember = new ModuleMember();
    // moduleMember->module = lib;
    // moduleMember->member = member;
}