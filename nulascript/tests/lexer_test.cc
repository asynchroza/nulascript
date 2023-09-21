#include "gtest/gtest.h"
#include "token.h"
#include "lexer.h"

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
    };

    for (const auto& testCase : testCases) {
            Lexer lexer = Lexer(testCase.first);

        for (const auto& expectedToken : testCase.second) {
            Token actualToken = lexer.getNextToken();

            EXPECT_EQ(expectedToken.type, actualToken.type);
            EXPECT_EQ(expectedToken.literal, actualToken.literal);
        }
    }

}
