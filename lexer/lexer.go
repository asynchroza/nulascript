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

// properly encapsulate the lexer
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

func (l *Lexer) readIdentifier() string {
	position := l.pos

	for isLetter(l.ch) {
		l.readChar()
	}

	return l.input[position:l.pos]
}

func (l *Lexer) GetNextToken() token.Token {
	var currentToken token.Token

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
		currentToken = newToken(token.POINTER, l.ch)
	case '&':
		currentToken = newToken(token.DEREF, l.ch)
	case '|':
		currentToken = newToken(token.PIPE, l.ch)
	case 0:
		currentToken.Literal = ""
		currentToken.Type = token.EOF
	default:
		if isLetter(l.ch) {
			currentToken.Literal = l.readIdentifier()
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

func isLetter(ch byte) bool {
	// add || ch == '_' if you want to make the language snake_case
	return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z'
}
