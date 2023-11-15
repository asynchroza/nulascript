#include "eval.h"

BooleanStorage* trueStorage = new BooleanStorage(true);
BooleanStorage* falseStorage = new BooleanStorage(false);
NilStorage* nilStorage = new NilStorage();

Storage* evaluate(Node* node);
Storage* evaluateSequence(std::vector<Statement*> statements);

bool checkTruthiness(Storage* storage) {
    if (storage == trueStorage) {
        return true;
    } else if (storage == falseStorage) {
        return false;
    } else if (storage == nilStorage) {
        return false;
    } else {
        std::cout << "[DEBUG]: Got a non boolean or nil value";
        return true;
    }
}

Storage* evaluateMinusExpression(Storage* rightExpression) {
    if (rightExpression->getType() == StorageType::INTEGER) {
        auto integer = dynamic_cast<IntegerStorage*>(rightExpression);
        return new IntegerStorage(-integer->value);
    }

    return nilStorage;
}

BooleanStorage* evaluateNotExpression(Storage* rightExpression) {
    if (rightExpression == trueStorage) {
        return falseStorage;
    } else if (rightExpression == falseStorage) {
        return trueStorage;
    } else if (rightExpression == nullptr) {
        return trueStorage;
    }

    return falseStorage;
}

Storage* evaluatePrefix(std::string op, Storage* rightExpression) {
    if (op == "!" || op == "not") {
        return evaluateNotExpression(rightExpression);
    } else if (op == "-") {
        return evaluateMinusExpression(rightExpression);
    }

    return nilStorage;
}

BooleanStorage* getBooleanReference(bool val) {
    return val ? trueStorage : falseStorage;
}

Storage* evaluateIntegerInfix(std::string op, Storage* leftExpression,
                              Storage* rightExpression) {
    auto left = dynamic_cast<IntegerStorage*>(leftExpression);
    auto right = dynamic_cast<IntegerStorage*>(rightExpression);

    if (op == "+") {
        return new IntegerStorage(left->value + right->value);
    } else if (op == "-") {
        return new IntegerStorage(left->value - right->value);
    } else if (op == "*") {
        return new IntegerStorage(left->value * right->value);
    } else if (op == "/") {
        return new IntegerStorage(left->value / right->value);
    } else if (op == "<") {
        return getBooleanReference(left->value < right->value);
    } else if (op == ">") {
        return getBooleanReference(left->value > right->value);
    } else if (op == "==" || op == "is") {
        return getBooleanReference(left->value == right->value);
    } else if (op == "!=" || op == "is not") {
        return getBooleanReference(left->value != right->value);
    } else if (op == ">=") {
        return getBooleanReference(left->value >= right->value);
    } else if (op == "<=") {
        return getBooleanReference(left->value <= right->value);
    }

    return nilStorage;
}

Storage* evaluateInfix(std::string op, Storage* leftExpression,
                       Storage* rightExpression) {
    if (leftExpression->getType() == StorageType::INTEGER &&
        rightExpression->getType() == StorageType::INTEGER) {
        return evaluateIntegerInfix(op, leftExpression, rightExpression);
    } else if (op == "==" || op == "is") {
        return getBooleanReference(leftExpression == rightExpression);
    } else if (op == "!=" || op == "is not") {
        return getBooleanReference(leftExpression != rightExpression);
    }

    return nilStorage;
}

Storage* evaluateIf(Conditional* expression) {
    auto condition = evaluate(expression->condition);

    if (checkTruthiness(condition)) {
        return evaluate(expression->currentBlock);
    } else if (expression->elseBlock) {
        return evaluate(expression->elseBlock);
    }

    return nilStorage;
}

template <typename T>
bool checkBase(T* passed, const std::type_info& expected) {
    return typeid(*passed) == expected;
}

Storage* evaluate(Node* node) {
    if (checkBase(node, typeid(Program))) {
        auto program = dynamic_cast<Program*>(node);
        return evaluateSequence(program->statements);
    } else if (checkBase(node, typeid(ExpressionStatement))) {
        auto statement = dynamic_cast<ExpressionStatement*>(node);
        return evaluate(statement->expression);
    } else if (checkBase(node, typeid(Integer))) {
        auto integer = dynamic_cast<Integer*>(node);
        return new IntegerStorage(integer->value);
    } else if (checkBase(node, typeid(Boolean))) {
        auto boolean = dynamic_cast<Boolean*>(node);
        return boolean->value ? trueStorage : falseStorage;
    } else if (checkBase(node, typeid(Prefix))) {
        auto prefix = dynamic_cast<Prefix*>(node);
        auto rightExpression = evaluate(prefix->right);
        return evaluatePrefix(prefix->op, rightExpression);
    } else if (checkBase(node, typeid(Infix))) {
        auto infix = dynamic_cast<Infix*>(node);
        auto leftExpression = evaluate(infix->left);
        auto rightExpression = evaluate(infix->right);
        return evaluateInfix(infix->op, leftExpression, rightExpression);
    } else if (checkBase(node, typeid(BlockStatement))) {
        auto block = dynamic_cast<BlockStatement*>(node);
        return evaluateSequence(block->statements);
    } else if (checkBase(node, typeid(Conditional))) {
        auto conditional = dynamic_cast<Conditional*>(node);
        return evaluateIf(conditional);
    } else if (checkBase(node, typeid(ReturnStatement))) {
        auto statement = dynamic_cast<ReturnStatement*>(node);
        auto result = evaluate(statement->returnValue);
        return new ReturnStorage(result);
    }

    return nilStorage;
}

Storage* evaluateSequence(std::vector<Statement*> statements) {
    Storage* result;

    for (auto statement : statements) {
        result = evaluate(statement);

        if (checkBase(result, typeid(ReturnStorage))) {
            auto returnVal = dynamic_cast<ReturnStorage*>(result);
            return returnVal->value;
        }
    }

    return result;
}
