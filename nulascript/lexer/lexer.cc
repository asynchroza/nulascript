#include "lexer.h"
#include "token.h"

Lexer::Lexer(const std::string& input)
    : input(input), pos(0), readPos(0), ch(0) {
    ReadChar();
}

void Lexer::ReadChar() {
    if (readPos >= static_cast<int>(input.size())) {
        ch = 0; // EOF
    } else {
        ch = input[readPos];
    }

    pos = readPos;
    readPos = readPos + 1;
}

std::string Lexer::ReadExtendedToken(TokenType tokenType) {
    int position = pos;

    bool (Lexer::*isCharFunc)(char) = nullptr;

    switch (tokenType) {
        case TokenType::INT:
            isCharFunc = &Lexer::IsDigit;
            break;
        case TokenType::IDENT:
            isCharFunc = &Lexer::IsLetter;
            break;
    }

    while ((this->*isCharFunc)(ch)) {
        ReadChar();
    }

    return input.substr(position, pos - position);
}

void Lexer::SkipOverWhitespace() {
    while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
        ReadChar();
    }
}

char Lexer::PeekNextChar() {
    if (readPos >= static_cast<int>(input.size())) {
        return 0; // EOF
    }

    return input[readPos];
}

Token Lexer::GetNextToken() {
    Token currentToken;

    SkipOverWhitespace();

    switch (ch) {
        case '=':
            if (PeekNextChar() == '=') {
                char savedCh = ch;
                ReadChar();
                currentToken = NewToken(TokenType::EQUAL, savedCh + ch);
            } else {
                currentToken = NewToken(TokenType::ASSIGN, ch);
            }
            break;
        case '+':
            currentToken = NewToken(TokenType::PLUS, ch);
            break;
        case '-':
            currentToken = NewToken(TokenType::MINUS, ch);
            break;
        case ',':
            currentToken = NewToken(TokenType::COMMA, ch);
            break;
        case ';':
            currentToken = NewToken(TokenType::SEMICOLON, ch);
            break;
        case '(':
            currentToken = NewToken(TokenType::LPAR, ch);
            break;
        case ')':
            currentToken = NewToken(TokenType::RPAR, ch);
            break;
        case '{':
            currentToken = NewToken(TokenType::LBRACE, ch);
            break;
        case '}':
            currentToken = NewToken(TokenType::RBRACE, ch);
            break;
        case '*':
            currentToken = NewToken(TokenType::ASTERISK, ch);
            break;
        case '&':
            currentToken = NewToken(TokenType::DEREF, ch);
            break;
        case '|':
            currentToken = NewToken(TokenType::PIPE, ch);
            break;
        case '!':
            if (PeekNextChar() == '=') {
                char savedCh = ch;
                ReadChar();
                currentToken = NewToken(TokenType::NOT_EQUAL, savedCh + ch);
            } else {
                currentToken = NewToken(TokenType::BANG, ch);
            }
            break;
        case '/':
            currentToken = NewToken(TokenType::SLASH, ch);
            break;
        case '<':
            currentToken = NewToken(TokenType::LT, ch);
            break;
        case '>':
            currentToken = NewToken(TokenType::GT, ch);
            break;
        case 0:
            currentToken.Literal = "";
            currentToken.Type = TokenType::EOF_TYPE;
            break;
        default:
            if (IsLetter(ch)) {
                currentToken.Literal = ReadExtendedToken(TokenType::IDENT);
                currentToken.Type = TokenLookup::LookupIdent(currentToken.Literal);
                return currentToken; // reading position and position are after the last character of the current identifier
            } else if (IsDigit(ch)) {
                currentToken.Type = TokenType::INT;
                currentToken.Literal = ReadExtendedToken(currentToken.Type);
                return currentToken;
            } else {
                currentToken = NewToken(TokenType::ILLEGAL, ch);
            }
    }

    ReadChar();
    return currentToken;
}

Token Lexer::NewToken(TokenType tokenType, char ch) {
    return Token{tokenType, std::string(1, ch)};
}

bool Lexer::IsLetter(char ch) {
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}

bool Lexer::IsDigit(char ch) {
    return ('0' <= ch && ch <= '9');
}
