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
				{token.ASTERISK, "*"},
				{token.DEREF, "&"},
				{token.PIPE, "|"},
			},
		}, {
			/*
				let numOne = 10;

				let numTwo = 20;

				let add = fn(x, y){
					x + y;
				}

				let result = add(numOne, numTwo)
			*/
			"let numOne = 10;\n\nlet numTwo = 20;\n\nlet add = fn(x, y)\n{\n\tx + y;\n}\n\nlet result = add(numOne, numTwo)",
			[]struct {
				expectedType    token.TokenType
				expectedLiteral string
			}{
				{token.LET, "let"},
				{token.IDENT, "numOne"},
				{token.ASSIGN, "="},
				{token.INT, "10"},
				{token.SEMICOLON, ";"},
				{token.LET, "let"},
				{token.IDENT, "numTwo"},
				{token.ASSIGN, "="},
				{token.INT, "20"},
				{token.SEMICOLON, ";"},
				{token.LET, "let"},
				{token.IDENT, "add"},
				{token.ASSIGN, "="},
				{token.FUNC, "fn"},
				{token.LPAR, "("},
				{token.IDENT, "x"},
				{token.COMMA, ","},
				{token.IDENT, "y"},
				{token.RPAR, ")"},
				{token.LBRACE, "{"},
				{token.IDENT, "x"},
				{token.PLUS, "+"},
				{token.IDENT, "y"},
				{token.SEMICOLON, ";"},
				{token.RBRACE, "}"},
				{token.LET, "let"},
				{token.IDENT, "result"},
				{token.ASSIGN, "="},
				{token.IDENT, "add"},
				{token.LPAR, "("},
				{token.IDENT, "numOne"},
				{token.COMMA, ","},
				{token.IDENT, "numTwo"},
				{token.RPAR, ")"},
			}},
		{"!-/*5;\n5 < 10 > 5;",
			[]struct {
				expectedType    token.TokenType
				expectedLiteral string
			}{
				{token.BANG, "!"},
				{token.MINUS, "-"},
				{token.SLASH, "/"},
				{token.ASTERISK, "*"},
				{token.INT, "5"},
				{token.SEMICOLON, ";"},
				{token.INT, "5"},
				{token.LT, "<"},
				{token.INT, "10"},
				{token.GT, ">"},
				{token.INT, "5"},
				{token.SEMICOLON, ";"},
			},
		}, {
			/*
								if (12 > 10) {
				       				return true;
				   				} else {
				       				return false;
								}
			*/
			"if (12 > 10) {\n\treturn true;\n} else {\n\treturn false;\n}",
			[]struct {
				expectedType    token.TokenType
				expectedLiteral string
			}{
				{token.IF, "if"},
				{token.LPAR, "("},
				{token.INT, "12"},
				{token.GT, ">"},
				{token.INT, "10"},
				{token.RPAR, ")"},
				{token.LBRACE, "{"},
				{token.RETURN, "return"},
				{token.TRUE, "true"},
				{token.SEMICOLON, ";"},
				{token.RBRACE, "}"},
				{token.ELSE, "else"},
				{token.LBRACE, "{"},
				{token.RETURN, "return"},
				{token.FALSE, "false"},
				{token.SEMICOLON, ";"},
				{token.RBRACE, "}"},
			},
		}, {
			"125 != 125;\n125 == 125;",
			[]struct {
				expectedType    token.TokenType
				expectedLiteral string
			}{
				{token.INT, "125"},
			},
		}}

	for _, testCase := range testCases {
		l := NewLexer(testCase.input)

		for iter, testToken := range testCase.tokenTests {
			var incomingToken token.Token = l.GetNextToken()

			if incomingToken.Type != testToken.expectedType {
				t.Fatalf("[%d][]: EXPECTED TOKEN: %q, RECEIVED TOKEN: %q", iter, testToken.expectedType, incomingToken.Type)
			}

			if incomingToken.Literal != testToken.expectedLiteral {
				t.Fatalf("[%d]: EXPECTED LITERAL: %q, RECEIVED LITERAL: %q", iter, testToken.expectedLiteral, incomingToken.Literal)
			}
		}
	}

}
