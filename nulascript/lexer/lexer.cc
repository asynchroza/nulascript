#include "lexer.h"
#include "token.h"

Lexer::Lexer(const std::string& input)
    : input(input), pos(0), readPos(0), ch(0), tokenLookup(TokenLookup()) {
    readChar();
}

void Lexer::readChar() {
    if (readPos >= static_cast<int>(input.size())) {
        ch = 0; // EOF
    } else {
        ch = input[readPos];
    }

    pos = readPos;
    readPos = readPos + 1;
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

    return input.substr(position, pos - position);
}

void Lexer::skipOverWhitespace() {
    while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
        readChar();
    }
}

char Lexer::peekNextChar() {
    if (readPos >= static_cast<int>(input.size())) {
        return 0; // EOF
    }

    return input[readPos];
}

Token Lexer::getNextToken() {
    Token currentToken;

    skipOverWhitespace();

    switch (ch) {
        case '=':
            if (peekNextChar() == '=') {
                char savedCh = ch;
                readChar();
                char tokenLiteral[3] = {savedCh, ch, '\0'};
                currentToken = newToken(TokenType::EQUAL, tokenLiteral);
            } else {
                currentToken = newToken(TokenType::ASSIGN, ch);
            }
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
            currentToken = newToken(TokenType::DEREF, ch);
            break;
        case '|':
            currentToken = newToken(TokenType::PIPE, ch);
            break;
        case '!':
            if (peekNextChar() == '=') {
                char savedCh = ch;
                readChar();
                char tokenLiteral[3] = {savedCh, ch, '\0'};
                currentToken = newToken(TokenType::NOT_EQUAL, tokenLiteral);
            } else {
                currentToken = newToken(TokenType::BANG, ch);
            }
            break;
        case '/':
            currentToken = newToken(TokenType::SLASH, ch);
            break;
        case '<':
            currentToken = newToken(TokenType::LT, ch);
            break;
        case '>':
            currentToken = newToken(TokenType::GT, ch);
            break;
        case 0:
            currentToken.literal = "";
            currentToken.type = TokenType::EOF_TYPE;
            break;
        default:
            if (isLetter(ch)) {
                currentToken.literal = readExtendedToken(TokenType::IDENT);
                currentToken.type = tokenLookup.lookupIdent(currentToken.literal);
                return currentToken; // reading position and position are after the last character of the current identifier
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

bool Lexer::isDigit(char ch) {
    return ('0' <= ch && ch <= '9');
}
