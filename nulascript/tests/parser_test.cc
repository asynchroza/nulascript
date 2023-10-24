#include "iostream"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "vector"
#include "gtest/gtest.h"

#define MULTILINE_STRING(s) #s

// utility functions

bool isCastableToDerivative(Statement* stmt, const std::type_info& type) {
    Statement* st;
    if (type == typeid(ReturnStatement)) {
        st = dynamic_cast<ReturnStatement*>(stmt);
    }
    if (!st)
        return false;
    return true;
}

void logParserErrors(std::vector<std::string> errors) {
    std::cout << "------LOGGING PARSER ERRORS------\n";

    for (std::string msg : errors) {
        std::cout << msg << "\n";
    }
}

// test suite

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

TEST(ParserSuite, TestLetStatement) {
    // Only the first two first two variables are correctly defined.

    // clang-format off
    std::string input =
        MULTILINE_STRING(
            let one = 1; 
            let two = 2; 
            let 123; 
            let something 128;
            );
    // clang-format on

    Lexer l(input);
    Parser p(l);
    Program* program = p.parseProgram();

    if (!program) {
        FAIL() << "Parsing the program returns a null pointer" << std::endl;
    }

    const int expectedStatements = 4;

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

TEST(ParserSuite, TestReturnStatement) {
    // clang-format off
    std::string input =
        MULTILINE_STRING(
            return 123; 
            return 10000; 
            return 20000000;
            );
    // clang-format on

    Lexer l(input);
    Parser p(l);
    Program* program = p.parseProgram();

    if (!p.getErrors().empty()) {
        logParserErrors(p.getErrors());
        FAIL() << "There are errors after parsing return statements";
    };

    const int expectedStatements = 3;

    if (program->statements.size() != expectedStatements) {
        FAIL() << "Parsed program doesn't match the number of input "
                  "statements: Got "
               << program->statements.size() << ", Expected "
               << expectedStatements << std::endl;
    }

    for (int i = 0; i < program->statements.size(); i++) {
        if (!isCastableToDerivative(program->statements[i],
                                    typeid(ReturnStatement))) {
            FAIL() << "Couldn't cast statement to ReturnStatement";
        }

        if (program->statements[i]->tokenLiteral() != "return") {
            FAIL() << "Token literal of return statement is not 'return'";
        }
    }
}

TEST(ParserSuite, TestAstToString) {
    // clang-format off
    std::string input = MULTILINE_STRING(
        let a = 5;
        return a;
    );

    std::string expectedResult = "let a = ;\nreturn ;\n";
    // clang-format on

    Lexer l(input);
    Parser p(l);
    Program* program = p.parseProgram();

    ASSERT_EQ(program->toString(), expectedResult);
}

TEST(ParserSuite, TestIdentifierExpression) {
    std::string input = "someIdentifier";

    Lexer l(input);
    Parser p(l);
    Program* program = p.parseProgram();

    if (program->statements.size() != 1) {
        FAIL() << "Program got " << program->statements.size()
               << " statements instead of 1";
    }

    if (!isCastableToDerivative(program->statements[0],
                                typeid(ExpressionStatement))) {
        FAIL() << "Statement is not of type ExpressionStatement";
    }

    if (!isCastableToDerivative(program->statements[0], typeid(Identifier))) {
        FAIL() << "Statement is not of type Identifier";
    }

    auto expression =
        dynamic_cast<ExpressionStatement*>(program->statements[0]);
    auto identifier = dynamic_cast<Identifier*>(expression->expression);

    if (identifier->value != input) {
        FAIL() << "Identifier has an incorrect value";
    }

    if (identifier->tokenLiteral() != input) {
        FAIL() << "Identifier has an incorrect token literal";
    }
}

TEST(ParserSuite, TestIntegerLiteralExpression) {
    std::string input = "10;";

    Lexer l(input);
    Parser p(l);
    Program* program = p.parseProgram();

    if (program->statements.size() != 1) {
        FAIL() << "Program got " << program->statements.size()
               << " statements instead of 1";
    }

    if (!isCastableToDerivative(program->statements[0],
                                typeid(ExpressionStatement))) {
        FAIL() << "Statement is not of type ExpressionStatement";
    }

    if (!isCastableToDerivative(program->statements[0], typeid(Integer))) {
        FAIL() << "Statement is not of type IntegerLiteral";
    }

    auto expression =
        dynamic_cast<ExpressionStatement*>(program->statements[0]);
    auto literal = dynamic_cast<Integer*>(expression->expression);

    if (literal->value != 10) {
        FAIL() << "IntegerLiteral has an incorrect value";
    }

    if (literal->tokenLiteral() != "10") {
        FAIL() << "IntegerLiteral has an incorrect token literal";
    }
}

TEST(ParserSuite, TestPrefixOperator) {
    union ValueUnion {
        int64_t* integerValue;
        std::string* stringValue;
    };

    struct PrefixTest {
        std::string input;
        std::string op;
        std::string literal;
    };

    std::vector<PrefixTest> prefixTests = {
        {"-69;", "-", "69"},
        {"!something;", "!", "something"},
    };

    for (const PrefixTest& t_case : prefixTests) {
        Lexer l(t_case.input);
        Parser p(l);
        Program* program = p.parseProgram();

        if (p.getErrors().size() != 0) {
            logParserErrors(p.getErrors());

            FAIL();
        }

        ASSERT_EQ(program->statements.size(), 1)
            << "program.Statements does not contain 1 statement. got="
            << program->statements.size();

        ExpressionStatement* statement =
            dynamic_cast<ExpressionStatement*>(program->statements[0]);

        if (!statement) {
            FAIL() << "program->statements[0] is not ExpressionStatement. got="
                   << typeid(program->statements[0]).name();
        }

        Prefix* expression = dynamic_cast<Prefix*>(statement->expression);
        if (!expression) {
            FAIL() << "statement is not Prefix. got="
                   << typeid(statement->expression).name();
        }

        ASSERT_EQ(expression->op, t_case.op)
            << "expression->operator is not '" << t_case.op
            << "'. got=" << expression->op;

        ASSERT_EQ(expression->right->tokenLiteral(), t_case.literal)
            << "expression->right->tokenLiteral() is not '" << t_case.literal
            << "', got=" << expression->right->tokenLiteral();
    }
}