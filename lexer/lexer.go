package lexer

import "nulascript/token"

type Lexer struct {
	input   string
	pos     int  // char current position
	readPos int  // used for peeking next char
	ch      byte // char byte under examination
}

func NewLexer(input string) lexerinterface {
	var l *Lexer = &Lexer{input: input}
	l.readChar()
	return l
}

// Used for properly encapsulating the lexer
type lexerinterface interface {
	GetNextToken() token.Token
}

func (l *Lexer) readChar() {
	if l.readPos >= len(l.input) {
		l.ch = 0 // EOF
	} else {
		l.ch = l.input[l.readPos]
	}

	l.pos = l.readPos
	l.readPos = l.readPos + 1
}

// Reads token which extend more than one character such as IDENTIFIERS and NUMBERS
func (l *Lexer) readExtendedToken(tokenType token.TokenType) string {
	position := l.pos

	var isCharFunc func(char byte) bool

	switch tokenType {
	case token.INT:
		isCharFunc = isDigit
	case token.IDENT:
		isCharFunc = isLetter
	}

	for isCharFunc(l.ch) {
		l.readChar()
	}

	return l.input[position:l.pos]
}

// Skip over whitespace characters until you land on a token
func (l *Lexer) skipOverWhitespace() {
	for l.ch == ' ' || l.ch == '\t' || l.ch == '\n' || l.ch == '\r' {
		l.readChar()
	}
}

func (l *Lexer) GetNextToken() token.Token {
	var currentToken token.Token

	l.skipOverWhitespace()

	switch l.ch {
	case '=':
		currentToken = newToken(token.ASSIGN, l.ch)
	case '+':
		currentToken = newToken(token.PLUS, l.ch)
	case '-':
		currentToken = newToken(token.MINUS, l.ch)
	case ',':
		currentToken = newToken(token.COMMA, l.ch)
	case ';':
		currentToken = newToken(token.SEMICOLON, l.ch)
	case '(':
		currentToken = newToken(token.LPAR, l.ch)
	case ')':
		currentToken = newToken(token.RPAR, l.ch)
	case '{':
		currentToken = newToken(token.LBRACE, l.ch)
	case '}':
		currentToken = newToken(token.RBRACE, l.ch)
	case '*':
		currentToken = newToken(token.ASTERISK, l.ch)
	case '&':
		currentToken = newToken(token.DEREF, l.ch)
	case '|':
		currentToken = newToken(token.PIPE, l.ch)
	case '!':
		currentToken = newToken(token.BANG, l.ch)
	case '/':
		currentToken = newToken(token.SLASH, l.ch)
	case '<':
		currentToken = newToken(token.LT, l.ch)
	case '>':
		currentToken = newToken(token.GT, l.ch)
	case 0:
		currentToken.Literal = ""
		currentToken.Type = token.EOF
	default:
		if isLetter(l.ch) {
			currentToken.Literal = l.readExtendedToken(token.IDENT)
			currentToken.Type = token.LookupIdent(currentToken.Literal)
			return currentToken // reading position and position are after the last character of current identifier
		} else if isDigit(l.ch) {
			currentToken.Type = token.INT
			currentToken.Literal = l.readExtendedToken(currentToken.Type)
			return currentToken
		} else {
			currentToken = newToken(token.ILLEGAL, l.ch)
		}
	}

	l.readChar()
	return currentToken
}

func newToken(tokenType token.TokenType, ch byte) token.Token {
	return token.Token{Type: tokenType, Literal: string(ch)}
}

// Check if current character is a letter.
// If you want to make the language snake_case,
// you need to consider underscores as alphabetic characters
func isLetter(ch byte) bool {
	return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z'
}

func isDigit(ch byte) bool {
	return '0' <= ch && ch <= '9'
}
