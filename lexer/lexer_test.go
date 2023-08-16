package lexer

import (
	"nulascript/token"
	"testing"
)

func TestGetNextToken(t *testing.T) {
	testCases := []struct {
		input      string
		tokenTests []struct {
			expectedType    token.TokenType
			expectedLiteral string
		}
	}{
		{`=+-,;(){}*&|`,
			[]struct {
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
			},
		},
	}

	for _, testCase := range testCases {
		l := NewLexer(testCase.input)

		for iter, testToken := range testCase.tokenTests {
			var incomingToken token.Token = l.GetNextToken()

			if incomingToken.Type != testToken.expectedType {
				t.Fatalf("[%d]: EXPECTED TOKEN: %q, RECEIVED TOKEN: %q", iter, testToken.expectedType, incomingToken.Type)
			}

			if incomingToken.Literal != testToken.expectedLiteral {
				t.Fatalf("[%d]: EXPECTED LITERAL: %q, RECEIVED LITERAL: %q", iter, testToken.expectedLiteral, incomingToken.Literal)
			}
		}
	}

}
