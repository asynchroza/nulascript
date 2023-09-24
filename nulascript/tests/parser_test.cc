#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "vector"
#include "gtest/gtest.h"

#define MULTILINE_STRING(s) #s

bool testLetStatement(Statement* statement, std::string ident) {
    if (LetStatement* castedStatement =
            dynamic_cast<LetStatement*>(statement)) {
        if (castedStatement->tokenLiteral() != "let") {
            std::cout
                << "Declaration statement is not using correct keyword: 'let'";
            return false;
        }

        if (castedStatement->name->value != ident) {
            std::cout << "LetStatement doesn't have correct name->value " +
                             ident + "; Got " + castedStatement->name->value +
                             " instead.";
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

    std::cout << "Did not receive LetStatement";
    return false;
}

TEST(ParserSuite, ParserTest) {
    std::string input = MULTILINE_STRING(let one = 1; let two = 2;);

    Lexer l(input);
    Parser p(l);
    Program* program = p.parseProgram();

    if (program == nullptr) {
        FAIL() << "Parsing the program returns a null pointer";
    }

    if (program->statements.size() != 2) {
        FAIL() << "Parsed program doesn't match the number of input statements";
    }

    std::vector<std::string> tests = {"one", "two"};

    for (int i = 0; i < tests.size(); i++) {
        Statement* statement = &program->statements[i];
        if (!testLetStatement(statement, tests[i])) {
            return;
        }
    }
}