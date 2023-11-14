#include "eval.h"

BooleanStorage* trueStorage = new BooleanStorage(true);
BooleanStorage* falseStorage = new BooleanStorage(false);
NilStorage* nilStorage = new NilStorage();

Storage* evaluate(Node* node);
Storage* evaluateSequence(std::vector<Statement*> statements);

Storage* evaluateMinusExpression(Storage* rightExpression) {
    if (rightExpression->getType() == StorageType::INTEGER) {
        auto integer = dynamic_cast<IntegerStorage*>(rightExpression);
        return new IntegerStorage(-integer->value);
    }

    return nullptr;
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

    return nullptr;
}

Storage* evaluate(Node* node) {
    if (auto program = dynamic_cast<Program*>(node)) {
        return evaluateSequence(program->statements);
    } else if (auto statement = dynamic_cast<ExpressionStatement*>(node)) {
        return evaluate(statement->expression);
    } else if (auto integer = dynamic_cast<Integer*>(node)) {
        return new IntegerStorage(integer->value);
    } else if (auto boolean = dynamic_cast<Boolean*>(node)) {
        return boolean->value ? trueStorage : falseStorage;
    } else if (auto prefix = dynamic_cast<Prefix*>(node)) {
        auto rightExpression = evaluate(prefix->right);
        return evaluatePrefix(prefix->op, rightExpression);
    }

    return nullptr;
}

Storage* evaluateSequence(std::vector<Statement*> statements) {
    Storage* result;

    for (auto statement : statements) {
        result = evaluate(statement);
    }

    return result;
}
