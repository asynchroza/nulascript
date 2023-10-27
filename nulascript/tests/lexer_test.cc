#include "lexer.h"
#include "token.h"
#include "gtest/gtest.h"

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
            MULTILINE_STRING(let numOne = 10; let numTwo = 20;
                             let add = fn(x, y) { x + y; }

                             let result = add(numOne, numTwo);),
            {
                {TokenType::LET, "let"},      {TokenType::IDENT, "numOne"},
                {TokenType::ASSIGN, "="},     {TokenType::INT, "10"},
                {TokenType::SEMICOLON, ";"},  {TokenType::LET, "let"},
                {TokenType::IDENT, "numTwo"}, {TokenType::ASSIGN, "="},
                {TokenType::INT, "20"},       {TokenType::SEMICOLON, ";"},
                {TokenType::LET, "let"},      {TokenType::IDENT, "add"},
                {TokenType::ASSIGN, "="},     {TokenType::FUNC, "fn"},
                {TokenType::LPAR, "("},       {TokenType::IDENT, "x"},
                {TokenType::COMMA, ","},      {TokenType::IDENT, "y"},
                {TokenType::RPAR, ")"},       {TokenType::LBRACE, "{"},
                {TokenType::IDENT, "x"},      {TokenType::PLUS, "+"},
                {TokenType::IDENT, "y"},      {TokenType::SEMICOLON, ";"},
                {TokenType::RBRACE, "}"},     {TokenType::LET, "let"},
                {TokenType::IDENT, "result"}, {TokenType::ASSIGN, "="},
                {TokenType::IDENT, "add"},    {TokenType::LPAR, "("},
                {TokenType::IDENT, "numOne"}, {TokenType::COMMA, ","},
                {TokenType::IDENT, "numTwo"}, {TokenType::RPAR, ")"},
            },
        },
        {"100 is not 100; 100 is 100; 100 >= 100; 100 <= 100;",
         {
             {TokenType::INT, "100"},
             {TokenType::IS_NOT, "is not"},
             {TokenType::INT, "100"},
             {TokenType::SEMICOLON, ";"},
             {TokenType::INT, "100"},
             {TokenType::IS, "is"},
             {TokenType::INT, "100"},
             {TokenType::SEMICOLON, ";"},
             {TokenType::INT, "100"},
             {TokenType::GOE, ">="},
             {TokenType::INT, "100"},
             {TokenType::SEMICOLON, ";"},
             {TokenType::INT, "100"},
             {TokenType::LOE, "<="},
             {TokenType::INT, "100"},
             {TokenType::SEMICOLON, ";"},

         }},
        {MULTILINE_STRING(
             if (12 > 10) { return true; } else { return false; }),
         {
             {TokenType::IF, "if"},
             {TokenType::LPAR, "("},
             {TokenType::INT, "12"},
             {TokenType::GT, ">"},
             {TokenType::INT, "10"},
             {TokenType::RPAR, ")"},
             {TokenType::LBRACE, "{"},
             {TokenType::RETURN, "return"},
             {TokenType::TRUE, "true"},
             {TokenType::SEMICOLON, ";"},
             {TokenType::RBRACE, "}"},
             {TokenType::ELSE, "else"},
             {TokenType::LBRACE, "{"},
             {TokenType::RETURN, "return"},
             {TokenType::FALSE, "false"},
             {TokenType::SEMICOLON, ";"},
             {TokenType::RBRACE, "}"},
         }}};

    for (const auto& testCase : testCases) {
        Lexer lexer(testCase.first);

        for (const auto& expectedToken : testCase.second) {
            Token actualToken = lexer.getNextToken();

            EXPECT_EQ(expectedToken.type, actualToken.type)
                << "Expected token literal is " + expectedToken.literal;
            EXPECT_EQ(expectedToken.literal, actualToken.literal);
        }
    }
}
