#include "lexer.h"
#include "token.h"
#include <iostream>

Lexer::Lexer(const std::string& input)
    : input(input), pos(0), readPos(0), ch(0), tokenLookup(TokenLookup()) {
    readChar();
}

void Lexer::readChar() {
    if (readPos >= input.size()) {
        ch = 0; // EOF
    } else {
        ch = input[readPos];
    }

    pos = readPos;
    readPos = readPos + 1;
}

std::string Lexer::readStringLiteral() {
    int position = pos + 1;
    while (true) {
        readChar();
        if (ch == '"' || ch == 0) {
            break;
        }
    }

    return input.substr(position, pos - position);
}

std::string Lexer::readExtendedToken(TokenType tokenType) {
    int position = pos;

    bool (Lexer::*isCharFunc)(char) = nullptr;

    switch (tokenType) {
    case TokenType::INT:
        isCharFunc = &Lexer::isDigit;
        break;
    case TokenType::IDENT:
        isCharFunc = &Lexer::isLetter;
        break;
    }

    while ((this->*isCharFunc)(ch)) {
        readChar();
    }

    auto currentBuffer = input.substr(position, pos - position);
    if (currentBuffer == "is") {
        if (currentBuffer == "is" &&
            position + (pos - position + 4) <= input.size() &&
            input.substr(position, pos - position + 4) == "is not") {
            readChar();
            readChar();
            readChar();
            readChar();
            return "is not";
        }
    }

    return input.substr(position, pos - position);
}

void Lexer::skipOverWhitespace() {
    while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
        readChar();
    }
}

void Lexer::skipLine() {
    while (ch != '\n' && peekNextChar() != 0) {
        readChar();
    }
}

char Lexer::peekNextChar() {
    if (readPos >= input.size()) {
        return 0; // EOF
    }

    return input[readPos];
}

Token Lexer::checkForEqualityOperator(char ch) {
    if (peekNextChar() == '=') {
        readChar();
        return newToken(TokenType::IS, "==");
    }

    return newToken(TokenType::ASSIGN, ch);
}

Token Lexer::getNextToken() {
    Token currentToken;

    skipOverWhitespace();

    switch (ch) {
    case '"':
        currentToken.type = TokenType::STRING;
        currentToken.literal = readStringLiteral();
        break;
    case '=':
        currentToken = checkForEqualityOperator(ch);
        break;
    case '+':
        currentToken = newToken(TokenType::PLUS, ch);
        break;
    case '-':
        currentToken = newToken(TokenType::MINUS, ch);
        break;
    case ',':
        currentToken = newToken(TokenType::COMMA, ch);
        break;
    case ';':
        currentToken = newToken(TokenType::SEMICOLON, ch);
        break;
    case '(':
        currentToken = newToken(TokenType::LPAR, ch);
        break;
    case ')':
        currentToken = newToken(TokenType::RPAR, ch);
        break;
    case '{':
        currentToken = newToken(TokenType::LBRACE, ch);
        break;
    case '}':
        currentToken = newToken(TokenType::RBRACE, ch);
        break;
    case '*':
        currentToken = newToken(TokenType::ASTERISK, ch);
        break;
    case '&':
        currentToken = newToken(TokenType::REF, ch);
        break;
    case '|':
        currentToken = newToken(TokenType::PIPE, ch);
        break;
    case '!':
        if (peekNextChar() == '=') {
            readChar();
            currentToken = newToken(TokenType::IS_NOT, "!=");
        } else {
            currentToken = newToken(TokenType::BANG_OR_NOT, ch);
        }
        break;
    case '/':
        currentToken = newToken(TokenType::SLASH, ch);
        break;
    case '<':
        currentToken =
            handleComparisonOperators(ch, TokenType::LT, TokenType::LOE);
        break;
    case '>':
        currentToken =
            handleComparisonOperators(ch, TokenType::GT, TokenType::GOE);
        break;
    case '#':
        currentToken = newToken(TokenType::HASHTAG, ch);
        skipLine();
        break;
    case 0:
        currentToken.literal = "";
        currentToken.type = TokenType::EOF_TYPE;
        break;
    default:
        if (isLetter(ch)) {
            currentToken.literal = readExtendedToken(TokenType::IDENT);
            if (currentToken.literal == "is not") {
                return newToken(TokenType::IS_NOT, "is not");
            }

            currentToken.type = tokenLookup.lookupIdent(currentToken.literal);
            return currentToken; // reading position and position are after the
                                 // last character of the current identifier
        } else if (isDigit(ch)) {
            currentToken.type = TokenType::INT;
            currentToken.literal = readExtendedToken(currentToken.type);
            return currentToken;
        } else {
            currentToken = newToken(TokenType::ILLEGAL, ch);
        }
    }

    readChar();
    return currentToken;
}

Token Lexer::newToken(TokenType tokenType, char ch) {
    return Token{tokenType, std::string(1, ch)};
}

Token Lexer::newToken(TokenType tokenType, const char* tokenLiteral) {
    return Token{tokenType, std::string(tokenLiteral)};
}

bool Lexer::isLetter(char ch) {
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}

bool Lexer::isDigit(char ch) { return ('0' <= ch && ch <= '9'); }

Token Lexer::handleComparisonOperators(char opChar, TokenType shortType,
                                       TokenType extendedType) {
    // reduce branching by not conditionally checking and inferring the extended
    // type
    if (peekNextChar() == '=') {
        char savedCh = ch;
        readChar();
        char tokenLiteral[3] = {savedCh, ch, '\0'};
        return newToken(extendedType, tokenLiteral);
    }

    return newToken(shortType, ch);
}
