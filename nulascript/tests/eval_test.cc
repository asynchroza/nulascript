#include "eval.h"
#include "iostream"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "vector"
#include "gtest/gtest.h"
#include <string>
#include <type_traits>

// #define MULTILINE_STRING(s) #s

Storage* getEvaluatedStorage(std::string input) {
    Lexer l(input);
    Parser p(l);
    auto program = p.parseProgram();
    auto environment = new Environment();

    return evaluate(program, environment);
}

TEST(EvalSuite, TestPrefixBangOperator) {
    struct Test {
        std::string input;
        bool expected;
    };

    std::vector<Test> tests = {{"not true", false}, {"!true", false},
                               {"!!true", true},    {"not not true", true},
                               {"!100", false},     {"not 100", false},
                               {"!!1000", true},    {"not not 1000", true}};

    for (auto test : tests) {
        auto result =
            dynamic_cast<BooleanStorage*>(getEvaluatedStorage(test.input));
        ASSERT_EQ(result->value, test.expected);
    }
}

TEST(EvalSuite, TestIntegerExpression) {
    struct Test {
        std::string input;
        int64_t expected;
    };

    std::vector<Test> tests = {
        {"10", 10}, {"-10", -10}, {"10 * 420 / 69 + ((69 / 420) * 100)", 60}};

    for (auto test : tests) {
        auto result =
            dynamic_cast<IntegerStorage*>(getEvaluatedStorage(test.input));
        ASSERT_EQ(result->value, test.expected);
    }
}

TEST(EvalSuite, TestIfStatement) {
    struct Test {
        std::string input;
        std::string expected;
    };

    std::vector<Test> tests = {{"if (true) { 69 }", "69"},
                               {"if (false) { 69 }", "nil"}};

    for (auto test : tests) {
        auto result = getEvaluatedStorage(test.input);
        ASSERT_EQ(result->evaluate(), test.expected);
    }
}

TEST(EvalSuite, TestReturn) {
    struct Test {
        std::string input;
        std::string expected;
    };

    std::vector<Test> tests = {
        {"return 69", "69"},
        {"if (420 > 69) { if (420 > 69) { return 420; } return 69; }", "420"}};

    for (auto test : tests) {
        auto result = getEvaluatedStorage(test.input);
        ASSERT_EQ(result->evaluate(), test.expected);
    }
}

TEST(EvalSuite, TestLet) {
    struct Test {
        std::string input;
        std::string expected;
    };

    std::vector<Test> tests = {
        {"let something = 420; let notSomething = 69; let result = something + "
         "notSomething; result;",
         "489"}};

    for (auto test : tests) {
        auto result = getEvaluatedStorage(test.input);
        ASSERT_EQ(result->evaluate(), test.expected);
    }
}

TEST(EvalSuite, TestFunction) {
    struct Test {
        std::string input;
        std::string expected;
    };

    std::vector<Test> tests = {
        {"let something = fn(x) {x;}; something(69);", "69"}};

    for (auto test : tests) {
        auto result = getEvaluatedStorage(test.input);
        ASSERT_EQ(result->evaluate(), test.expected);
    }
}