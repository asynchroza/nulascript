#include "iostream"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "vector"
#include "gtest/gtest.h"
#include <string>
#include <type_traits>

#define MULTILINE_STRING(s) #s

// utility functions

bool isNumber(std::string str) {
    try {
        std::stoi(str);
        return true;
    } catch (...) {
        return false;
    }
}

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

bool checkLiteral(Statement* expression, std::string value) {
    Identifier* identifier = dynamic_cast<Identifier*>(expression);
    std::cout << typeid(identifier).name() << std::endl;

    if (!identifier) {
        ExpressionStatement* exp =
            dynamic_cast<ExpressionStatement*>(expression);

        if (!exp) {
            std::cout << "literal is not ExpressionStatement. got="
                      << typeid(*expression).name() << std::endl;
            return false;
        }

        identifier = dynamic_cast<Identifier*>(exp->expression);
    }

    if (!identifier) {
        std::cout << "expression is not Identifier. got="
                  << typeid(*expression).name() << std::endl;
        return false;
    }

    if (identifier->value != value) {
        std::cout << "identifier->value not=" << value
                  << ". got=" << identifier->value;
        return false;
    }

    if (identifier->tokenLiteral() != value) {
        std::cout << "identifier->tokenLiteral not=" << value
                  << ". got=" << identifier->tokenLiteral();
        return false;
    }

    return true;
}

bool checkLiteral(Statement* expression, bool value) {
    ExpressionStatement* exp = dynamic_cast<ExpressionStatement*>(expression);

    if (!exp) {
        std::cout << "literal is not ExpressionStatement. got="
                  << typeid(*expression).name() << std::endl;
        return false;
    }

    Boolean* boolean = dynamic_cast<Boolean*>(exp->expression);

    if (!boolean) {
        std::cout << "boolean is not Boolean. got="
                  << typeid(*expression).name() << std::endl;
        return false;
    }

    if (boolean->value != value) {
        std::cout << "boolean->value not=" << value
                  << ". got=" << boolean->value;
        return false;
    }

    return true;
}

bool checkLiteral(Statement* expression, int value) {
    Integer* integer = dynamic_cast<Integer*>(expression);

    if (!integer) {
        ExpressionStatement* exp =
            dynamic_cast<ExpressionStatement*>(expression);
        if (!exp) {
            std::cout << "literal is not ExpressionStatement. got="
                      << typeid(*expression).name() << std::endl;
            return false;
        }

        integer = dynamic_cast<Integer*>(exp->expression);

        if (!integer) {
            std::cout << "expression is not Integer. got="
                      << typeid(*expression).name() << std::endl;
            return false;
        }
    }

    if (std::stoi(integer->tokenLiteral()) != value) {
        std::cout << "integer->tokenLiteral() not=" << value
                  << ". got=" << integer->tokenLiteral();
        return false;
    }

    if (integer->value != value) {
        std::cout << "integer->value not=" << value
                  << ". got=" << integer->value;
        return false;
    }

    return true;
}

bool testInfixExpression(Expression* exp, const std::string& op,
                         const std::string& left, const std::string& right) {
    Infix* opExp = dynamic_cast<Infix*>(exp);

    if (!opExp) {
        return false;
    }

    if (opExp->left->tokenLiteral() != left) {
        return false;
    }

    if (opExp->op != op) {
        return false;
    }

    if (opExp->right->tokenLiteral() != right) {
        return false;
    }

    return true;
}

struct IdentTest {
    std::string tokenOfIdent;
    std::string assignedVal;
};

// test suite
bool testLetStatement(Statement* statement, IdentTest ident) {
    LetStatement* castedStatement = dynamic_cast<LetStatement*>(statement);

    if (!castedStatement) {
        std::cout << "Object is not of type LetStatement" << std::endl;
        return false;
    }

    if (castedStatement->tokenLiteral() != "def") {
        std::cout << "Declaration statement is not using correct keyword: 'def'"
                  << std::endl;
        return false;
    }

    if (castedStatement->value->tokenLiteral() != ident.assignedVal) {
        std::cout << "LetStatement value's token literal doesn't match " +
                         ident.assignedVal + "; Got " +
                         castedStatement->name->tokenLiteral() + "."
                  << std::endl;
        return false;
    }

    if (castedStatement->name->tokenLiteral() != ident.tokenOfIdent) {
        std::cout << "LetStatement name's token literal doesn't match " +
                         ident.tokenOfIdent + "; Got " +
                         castedStatement->name->tokenLiteral() + "."
                  << std::endl;
        return false;
    }

    return true;
}

TEST(ParserSuite, TestLetStatement) {
    // Only the first two variables are correctly defined.

    // clang-format off
    std::string input =
        MULTILINE_STRING(
            def one = 1; 
            def two = 2; 
            def 123; 
            def something 128;
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

    std::vector<IdentTest> tests = {{"one", "1"}, {"two", "2"}};

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
        def a = 5;
        return a;
    );

    std::string expectedResult = "def a = 5;return a;";
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

    ASSERT_TRUE(
        checkLiteral(program->statements[0], std::string("someIdentifier")));
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

    ASSERT_TRUE(checkLiteral(program->statements[0], 10));
}

TEST(ParserSuite, TestBooleanLiteralExpression) {
    std::string input = "true;";

    Lexer l(input);
    Parser p(l);
    Program* program = p.parseProgram();

    if (program->statements.size() != 1) {
        FAIL() << "Program got " << program->statements.size()
               << " statements instead of 1";
    }

    ASSERT_TRUE(checkLiteral(program->statements[0], true));
}

TEST(ParserSuite, TestPrefixOperator) {

    struct PrefixTest {
        std::string input;
        std::string op;
        std::string literal;
    };

    std::vector<PrefixTest> prefixTests = {
        {"-69;", "-", "69"},
        {"!something;", "!", "something"},
        {"not something;", "not", "something"}};

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

TEST(ParserSuite, TestInfixOperator) {
    struct InfixTest {
        std::string input;
        int64_t leftVal;
        std::string op;
        int64_t rightVal;
    };

    std::vector<InfixTest> infixTests = {
        {"69 + 69;", 69, "+", 69},   {"69 - 69;", 69, "-", 69},
        {"69 * 69;", 69, "*", 69},   {"69 / 69;", 69, "/", 69},
        {"69 > 69;", 69, ">", 69},   {"69 < 69;", 69, "<", 69},
        {"69 == 69;", 69, "==", 69}, {"69 != 69;", 69, "!=", 69},
    };

    for (const InfixTest& t_case : infixTests) {
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

        Infix* expression = dynamic_cast<Infix*>(statement->expression);
        if (!expression) {
            FAIL() << "statement is not Infix. got="
                   << typeid(statement->expression).name();
        }

        ASSERT_EQ(expression->op, t_case.op)
            << "expression->operator is not '" << t_case.op
            << "'. got=" << expression->op;

        ASSERT_EQ(expression->right->tokenLiteral(),
                  std::to_string(t_case.rightVal))
            << "expression->right->tokenLiteral() is not '" << t_case.rightVal
            << "', got=" << expression->right->tokenLiteral();

        ASSERT_TRUE(isNumber(expression->right->tokenLiteral()))
            << "Right expression value is not number";

        ASSERT_EQ(expression->left->tokenLiteral(),
                  std::to_string(t_case.leftVal))
            << "expression->right->tokenLiteral() is not '" << t_case.leftVal
            << "', got=" << expression->left->tokenLiteral();

        ASSERT_TRUE(isNumber(expression->left->tokenLiteral()))
            << "Left expression value is not number";
    }
}

TEST(ParserSuite, TestOperatorPrecedence) {
    struct PrecedenceTest {
        std::string input;
        std::string output;
    };

    std::vector<PrecedenceTest> precedenceTests = {
        {"a + b - c", "((a + b) - c)"},
        {"a + b + c", "((a + b) + c)"},
        {"a - b + c", "((a - b) + c)"},
        {"a - b - c", "((a - b) - c)"},
        {"a + b + c + d / e + f", "((((a + b) + c) + (d / e)) + f)"},
        {"a * b + c + d / e + f", "((((a * b) + c) + (d / e)) + f)"},
        {"a + b * c == a * d + p * q",
         "((a + (b * c)) == ((a * d) + (p * q)))"},
        {"a + b * c is not a * d + p * q",
         "((a + (b * c)) is not ((a * d) + (p * q)))"},
        {"false == a + b", "(false == (a + b))"},
        {"a + (b + c) + d", "((a + (b + c)) + d)"},
        {"multiply(a, b + c + d, d * p * (c / d))",
         "multiply(a, ((b + c) + d), ((d * p) * (c / d)))"},
        {"10 * 420 / 69 + ((69 / 420) * 100)",
         "(((10 * 420) / 69) + ((69 / 420) * 100))"}};

    for (auto test : precedenceTests) {
        Lexer l(test.input);
        Parser p(l);
        auto program = p.parseProgram();

        ASSERT_EQ(program->toString(), test.output);
    };
}

TEST(ParserSuite, TestConditionalExpression) {
    std::string input = "if (a > b) { 100 } else { 200 }";

    Lexer l(input);
    Parser p(l);
    Program* program = p.parseProgram();

    if (!p.getErrors().empty()) {
        // logParserErrors(p.getErrors());
        FAIL() << "There are errors after parsing if expression";
    }

    if (program->statements.size() != 1) {
        FAIL() << "program.Statements does not contain 1 statement. got="
               << program->statements.size();
    }

    ExpressionStatement* stmt =
        dynamic_cast<ExpressionStatement*>(program->statements[0]);

    if (!stmt) {
        FAIL() << "program.Statements[0] is not ExpressionStatement. got="
               << typeid(program->statements[0]).name();
    }

    Conditional* exp = dynamic_cast<Conditional*>(stmt->expression);

    if (!exp) {
        FAIL() << "exp is not Conditional. got="
               << typeid(stmt->expression).name();
    }

    ASSERT_TRUE(!testInfixExpression(exp->condition, "a", ">", "b"));

    if (exp->currentBlock->statements.size() != 1) {
        FAIL() << "currentBlock is not 1 statement. got="
               << exp->currentBlock->statements.size();
    }

    ExpressionStatement* currentBlock =
        dynamic_cast<ExpressionStatement*>(exp->currentBlock->statements[0]);

    if (!currentBlock) {
        FAIL() << "current block statements[0] is not ExpressionStatement. got "
                  "instead ->"
               << typeid(exp->currentBlock->statements[0]).name();
    }

    ExpressionStatement* elseBlock =
        dynamic_cast<ExpressionStatement*>(exp->elseBlock->statements[0]);

    if (!elseBlock) {
        FAIL() << "else block statements[0] is not ExpressionStatement. got "
                  "instead -> "
               << typeid(exp->elseBlock->statements[0]).name();
    }

    ASSERT_TRUE(checkLiteral(exp->currentBlock->statements[0], 100));
    ASSERT_TRUE(checkLiteral(exp->elseBlock->statements[0], 200));
}

TEST(ParserSuite, TestFunction) {
    std::string input = "func(argOne, argTwo) { argOne + argTwo; }";

    Lexer l(input);
    Parser p(l);
    Program* program = p.parseProgram();

    if (!p.getErrors().empty()) {
        // logParserErrors(p.getErrors());
        FAIL() << "There are errors after parsing function";
    }

    if (program->statements.size() != 1) {
        FAIL() << "program.Statements does not contain 1 statement. got="
               << program->statements.size();
    }

    ExpressionStatement* stmt =
        dynamic_cast<ExpressionStatement*>(program->statements[0]);

    if (!stmt) {
        FAIL() << "program.Statements[0] is not ExpressionStatement. got="
               << typeid(program->statements[0]).name();
    }

    Function* exp = dynamic_cast<Function*>(stmt->expression);

    if (!exp) {
        FAIL() << "exp is not Function. got="
               << typeid(stmt->expression).name();
    }

    // TODO: This will break once actual value is allocated
    ASSERT_EQ(exp->arguments[0]->value, std::string("argOne"));
    ASSERT_EQ(exp->arguments[1]->value, std::string("argTwo"));

    ExpressionStatement* exp_stmt =
        dynamic_cast<ExpressionStatement*>(exp->code->statements[0]);
    Infix* infix = dynamic_cast<Infix*>(exp_stmt->expression);
    if (infix) {
        if (infix->op != "+" || infix->left->toString() != "argOne" ||
            infix->right->toString() != "argTwo") {
            FAIL() << "Either left, right or the operator was not properly "
                      "parsed within the infix expression";
        }
    } else {
        FAIL() << "Couldn't properly parse Infix expression nested in "
                  "ExpressionStatement";
    }
}

TEST(ParserSuite, TestInvocation) {
    std::string input = "multiply(1, 1 + 2, 1 * 2);";

    Lexer l(input);
    Parser p(l);
    Program* program = p.parseProgram();

    if (!p.getErrors().empty()) {
        // logParserErrors(p.getErrors());
        FAIL() << "There are errors after parsing invocation";
    }

    if (program->statements.size() != 1) {
        FAIL() << "program.Statements does not contain 1 statement. got="
               << program->statements.size();
    }

    ExpressionStatement* stmt =
        dynamic_cast<ExpressionStatement*>(program->statements[0]);

    if (!stmt) {
        FAIL() << "program.Statements[0] is not ExpressionStatement. got="
               << typeid(program->statements[0]).name();
    }

    Invocation* exp = dynamic_cast<Invocation*>(stmt->expression);

    if (!exp) {
        FAIL() << "exp is not Invocation. got="
               << typeid(stmt->expression).name();
    }
    EXPECT_EQ(exp->arguments.size(), 3);
    EXPECT_EQ(exp->arguments[0]->tokenLiteral(), "1");
}

TEST(ParserSuite, TestReference) {
    std::string input = "&something";

    Lexer l(input);
    Parser p(l);
    Program* program = p.parseProgram();

    if (program->statements.size() != 1) {
        FAIL() << "Program got " << program->statements.size()
               << " statements instead of 1";
    }

    auto stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
    if (!stmt) {
        FAIL() << "Not correct type" << std::endl;
    }

    auto ref = dynamic_cast<Reference*>(stmt->expression);
    if (!ref) {
        FAIL() << "Not correct type" << std::endl;
    }

    ASSERT_EQ(ref->referencedIdentifier, "something");
    ASSERT_EQ(ref->tokenLiteral(), "&");
}

TEST(ParserSuite, TestForLoop) {
    std::string input = "for(def i = 5; i < 5; i + 2) { def i = i + 5; }";

    Lexer l(input);
    Parser p(l);
    Program* program = p.parseProgram();

    if (program->statements.size() != 1) {
        FAIL() << "Program got " << program->statements.size()
               << " statements instead of 1";
    }

    auto stmt = dynamic_cast<ExpressionStatement*>(program->statements[0]);
    if (!stmt) {
        FAIL() << "Not correct type" << std::endl;
    }

    auto fl = dynamic_cast<ForLoop*>(stmt->expression);
    if (!fl) {
        FAIL() << "Not correct type" << std::endl;
    }

    ASSERT_EQ(fl->definition.variable->toString(), "def i = 5;");
    ASSERT_EQ(fl->definition.conditional->toString(), "(i < 5)");
    auto cast = dynamic_cast<Infix*>(fl->definition.conditional);
    ASSERT_EQ(cast->left->toString(), "i");
    ASSERT_EQ(fl->definition.increment->toString(), "(i + 2)");
    ASSERT_EQ(fl->code->toString(), "def i = (i + 5);");
}
