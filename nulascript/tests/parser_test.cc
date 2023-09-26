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
        std::cout << "Object is not of type LetStatement";
        return false;
    }

    if (castedStatement->tokenLiteral() != "let") {
        std::cout
            << "Declaration statement is not using correct keyword: 'let'";
        return false;
    }

    if (castedStatement->name->value != ident) {
        std::cout << "LetStatement doesn't have correct name->value " + ident +
                         "; Got " + castedStatement->name->value + " instead.";
        return false;
    }

    if (castedStatement->name->tokenLiteral() != ident) {
        std::cout << "LetStatement name's token literal doesn't match " +
                         ident + "; Got " +
                         castedStatement->name->tokenLiteral() + ".";
        return false;
    }

    return true;
}

TEST(ParserSuite, ParserTest) {
    std::string input = MULTILINE_STRING(let one = 1; let two = 2;);

    Lexer l(input);
    Parser p(l);
    Program* program = p.parseProgram();

    if (!program) {
        FAIL() << "Parsing the program returns a null pointer";
    }

    int expectedStatements = 2;

    if (program->statements.size() != expectedStatements) {
        FAIL() << "Parsed program doesn't match the number of input "
                  "statements: Got "
               << program->statements.size() << ", Expected "
               << expectedStatements;
    }

    std::vector<std::string> tests = {"one", "two"};

    for (int i = 0; i < tests.size(); i++) {
        Statement* statement = program->statements[i];

        // ! downcasting here works as expected
        LetStatement* stmt = dynamic_cast<LetStatement*>(statement);

        // ! but the downcasting results in a segfault when done in
        // testLetStatement
        if (!testLetStatement(statement, tests[i])) {
            FAIL();
        }
    }
}