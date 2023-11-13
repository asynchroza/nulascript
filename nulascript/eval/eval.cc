#include "eval.h"

Storage* evaluate(Node* node);
Storage* evaluateSequence(std::vector<Statement*> statements);

Storage* evaluate(Node* node) {
    if (auto program = dynamic_cast<Program*>(node)) {
        return evaluateSequence(program->statements);
    } else if (auto statement = dynamic_cast<ExpressionStatement*>(node)) {
        return evaluate(statement->expression);
    } else if (auto integer = dynamic_cast<Integer*>(node)) {
        return new IntegerStorage(integer->value);
    } else if (auto boolean = dynamic_cast<Boolean*>(node)) {
        return new BooleanStorage(boolean->value);
    } else if (auto nil = dynamic_cast<NilStorage*>(node)) {
        return new NilStorage();
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
