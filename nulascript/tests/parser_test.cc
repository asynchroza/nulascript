#include "iostream"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "vector"
#include "gtest/gtest.h"

#define MULTILINE_STRING(s) #s

bool testLetStatement(Statement* statement, std::string ident) {
    std::cout << "HERE" << std::endl;
    // ! segfault when dynamically casting to LetStatement
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

    if (program == nullptr) {
        FAIL() << "Parsing the program returns a null pointer";
    }

    int expectedStatements = 2;
    // std::cout << program->statements.at(0)->tokenLiteral();

    if (program->statements.size() != expectedStatements) {
        FAIL() << "Parsed program doesn't match the number of input "
                  "statements: Got "
               << program->statements.size() << ", Expected "
               << expectedStatements;
    }

    std::vector<std::string> tests = {"one", "two"};

    for (int i = 0; i < tests.size(); i++) {
        Statement* statement = program->statements[i];
        if (!testLetStatement(statement, tests[i])) {
            FAIL();
        }
    }
}