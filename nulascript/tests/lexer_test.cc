#include "gtest/gtest.h"
#include "token.h"
#include "lexer.h"

#define MULTILINE_STRING(s) #s

TEST(LexerSuite, LexerTest) {
    std::vector<std::pair<std::string, std::vector<Token>>> testCases = {
        {
            "=+-,;(){}*&|",
            {
                {TokenType::ASSIGN, "="},
                {TokenType::PLUS, "+"},
                {TokenType::MINUS, "-"},
                {TokenType::COMMA, ","},
                {TokenType::SEMICOLON, ";"},
                {TokenType::LPAR, "("},
                {TokenType::RPAR, ")"},
                {TokenType::LBRACE, "{"},
                {TokenType::RBRACE, "}"},
                {TokenType::ASTERISK, "*"},
                {TokenType::DEREF, "&"},
                {TokenType::PIPE, "|"},
            },
        },
        {
            MULTILINE_STRING(let numOne = 10;
            let numTwo = 20;
			let add = fn(x, y){
				x + y;
			}

			let result = add(numOne, numTwo);),
            {
                {TokenType::LET, "let"},
				{TokenType::IDENT, "numOne"},
				{TokenType::ASSIGN, "="},
				{TokenType::INT, "10"},
				{TokenType::SEMICOLON, ";"},
				{TokenType::LET, "let"},
				{TokenType::IDENT, "numTwo"},
				{TokenType::ASSIGN, "="},
				{TokenType::INT, "20"},
				{TokenType::SEMICOLON, ";"},
				{TokenType::LET, "let"},
				{TokenType::IDENT, "add"},
				{TokenType::ASSIGN, "="},
				{TokenType::FUNC, "fn"},
				{TokenType::LPAR, "("},
				{TokenType::IDENT, "x"},
				{TokenType::COMMA, ","},
				{TokenType::IDENT, "y"},
				{TokenType::RPAR, ")"},
				{TokenType::LBRACE, "{"},
				{TokenType::IDENT, "x"},
				{TokenType::PLUS, "+"},
				{TokenType::IDENT, "y"},
				{TokenType::SEMICOLON, ";"},
				{TokenType::RBRACE, "}"},
				{TokenType::LET, "let"},
				{TokenType::IDENT, "result"},
				{TokenType::ASSIGN, "="},
				{TokenType::IDENT, "add"},
				{TokenType::LPAR, "("},
				{TokenType::IDENT, "numOne"},
				{TokenType::COMMA, ","},
				{TokenType::IDENT, "numTwo"},
				{TokenType::RPAR, ")"},
            }
        }
    };

    for (const auto& testCase : testCases) {
            Lexer lexer(testCase.first);

        for (const auto& expectedToken : testCase.second) {
            Token actualToken = lexer.getNextToken();

            EXPECT_EQ(expectedToken.type, actualToken.type);
            EXPECT_EQ(expectedToken.literal, actualToken.literal);
        }
    }

}
