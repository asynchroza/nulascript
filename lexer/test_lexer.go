package lexer

import (
	"nulascript/token"
	"testing"
)

func TestGetNextToken(t *testing.T) {
	input := `=+-,;(){}*&|`

	tokenTests := []struct {
		expectedType    token.TokenType
		expectedLiteral string
	}{
		{token.ASSIGN, "="},
		{token.PLUS, "+"},
		{token.MINUS, "-"},
		{token.COMMA, ","},
		{token.SEMICOLON, ";"},
		{token.LPAR, "("},
		{token.RPAR, ")"},
		{token.LBRACE, "{"},
		{token.RBRACE, "}"},
		{token.POINTER, "*"},
		{token.DEREF, "&"},
		{token.PIPE, "|"},
	}

	l := NewLexer(input)

	for _, testToken := range tokenTests {
		var incomingToken token.Token = l.GetNextToken()

		if incomingToken.Type != testToken.expectedType {
			t.Fatalf("EXPECTED TOKEN: %q, RECEIVED TOKEN: %q", testToken.expectedType, incomingToken.Type)
		}

		if incomingToken.Literal != testToken.expectedLiteral {
			t.Fatalf("EXPECTED LITERAL: %q, RECEIVED LITERAL: %q", testToken.expectedLiteral, incomingToken.Literal)
		}
	}
}
