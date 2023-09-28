#include "iostream"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "vector"
#include "gtest/gtest.h"

#define MULTILINE_STRING(s) #s

bool testLetStatement(Statement* statement, std::string ident) {
    LetStatement* castedStatement = dynamic_cast<LetStatement*>(statement);

    if (!castedStatement) {
        std::cout << "Object is not of type LetStatement" << std::endl;
        return false;
    }

    if (castedStatement->tokenLiteral() != "let") {
        std::cout << "Declaration statement is not using correct keyword: 'let'"
                  << std::endl;
        return false;
    }

    if (castedStatement->name->token.literal != ident) {
        std::cout
            << "LetStatement's identifier has incorrect literal: Expected " +
                   ident + "; Got " + castedStatement->name->token.literal +
                   " instead."
            << std::endl;
        return false;
    }

    if (castedStatement->name->tokenLiteral() != ident) {
        std::cout << "LetStatement name's token literal doesn't match " +
                         ident + "; Got " +
                         castedStatement->name->tokenLiteral() + "."
                  << std::endl;
        return false;
    }

    return true;
}

void testCasting(Statement* stmt) {
    auto st = dynamic_cast<LetStatement*>(stmt);
}

TEST(ParserSuite, ParserTest) {
    std::string input =
        MULTILINE_STRING(let one = 1; let two = 2; let 123; let something 128;);

    Lexer l(input);
    Parser p(l);
    Program* program = p.parseProgram();

    if (!program) {
        FAIL() << "Parsing the program returns a null pointer" << std::endl;
    }

    int expectedStatements = 2;

    if (program->statements.size() != expectedStatements) {
        FAIL() << "Parsed program doesn't match the number of input "
                  "statements: Got "
               << program->statements.size() << ", Expected "
               << expectedStatements << std::endl;
    }

    std::vector<std::string> tests = {"one", "two"};

    for (int i = 0; i < tests.size(); i++) {
        Statement* statement = program->statements[i];

        if (!testLetStatement(statement, tests[i])) {
            FAIL();
        }
    }

    std::vector<std::string> expectedErrors = {
        "Expected token::type 2; Got token::type 3",
        "Expected token::type 4; Got token::type 3"};
    std::vector<std::string> actualErrors = p.getErrors();
    bool shouldFail = false;

    for (int i = 0; i < expectedErrors.size(); i++) {
        if (actualErrors[i] != expectedErrors[i]) {
            std::cout << "GOT - \"" << actualErrors[i] << "\"; EXPECTED - \""
                      << expectedErrors[i] << "\"" << std::endl;
            shouldFail = true;
        }
    }

    // display all non-matching error messages before failing
    if (shouldFail)
        FAIL();
}