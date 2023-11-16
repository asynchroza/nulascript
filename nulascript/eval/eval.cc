#include "eval.h"

BooleanStorage* trueStorage = new BooleanStorage(true);
BooleanStorage* falseStorage = new BooleanStorage(false);
NilStorage* nilStorage = new NilStorage();

Storage* evaluate(Node* node, Environment* env);
Storage* evaluateProgramStatements(std::vector<Statement*> statements,
                                   Environment* env);

template <typename T>
bool checkBase(T* passed, const std::type_info& expected) {
    return typeid(*passed) == expected;
}

bool checkTruthiness(Storage* storage) {
    if (storage == trueStorage) {
        return true;
    } else if (storage == falseStorage) {
        return false;
    } else if (storage == nilStorage) {
        return false;
    } else {
        // TODO: extend logic for verifying truthiness
        return true;
    }
}

ErrorStorage* createError(std::string message) {
    return new ErrorStorage(message);
}

Storage* evaluateMinusExpression(Storage* rightExpression) {
    if (rightExpression->getType() == StorageType::INTEGER) {
        auto integer = dynamic_cast<IntegerStorage*>(rightExpression);
        return new IntegerStorage(-integer->value);
    }

    return createError("Unknown operator -" +
                       parseStorageTypeToString(rightExpression->getType()));
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

    return createError("Unknown operator " + op +
                       parseStorageTypeToString(rightExpression->getType()));
}

BooleanStorage* getBooleanReference(bool val) {
    return val ? trueStorage : falseStorage;
}

bool isErrorStorage(Storage* storage) {
    return checkBase(storage, typeid(ErrorStorage));
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
    } else if (leftExpression->getType() == StorageType::STRING &&
               rightExpression->getType() == StorageType::STRING && op == "+") {
        return new StringStorage(leftExpression->evaluate() +
                                 rightExpression->evaluate());
    } else if (op == "==" || op == "is") {
        return getBooleanReference(leftExpression == rightExpression);
    } else if (op == "!=" || op == "is not") {
        return getBooleanReference(leftExpression != rightExpression);
    } else if (leftExpression->getType() != rightExpression->getType()) {
        std::string errorMessage = "";

        errorMessage = "Type missmatch. Left side is " +
                       parseStorageTypeToString(leftExpression->getType()) +
                       " and right side is " +
                       parseStorageTypeToString(rightExpression->getType());

        return createError(errorMessage);
    }

    return createError("Unkown operator " + leftExpression->evaluate() + " " +
                       op + " " + rightExpression->evaluate());
}

Storage* evaluateIf(Conditional* expression, Environment* env) {
    auto condition = evaluate(expression->condition, env);
    if (isErrorStorage(condition))
        return condition;

    if (checkTruthiness(condition)) {
        return evaluate(expression->currentBlock, env);
    } else if (expression->elseBlock) {
        return evaluate(expression->elseBlock, env);
    }

    return nilStorage;
}

Storage* evaluateBlockStatement(std::vector<Statement*> statements,
                                Environment* env) {
    // TODO: (low prio) accept BlockStatement as an argument instead of the
    Storage* result;

    for (auto stmt : statements) {
        result = evaluate(stmt, env);

        const StorageType resultType = result->getType();
        if (result && resultType == StorageType::ERROR ||
            resultType == StorageType::RETURN) {
            return result;
        }
    }

    return result;
}

std::vector<Storage*> evaluateArgs(std::vector<Expression*> arguments,
                                   Environment* env) {

    std::vector<Storage*> evaluatedArgs;
    Storage* evaluated;

    for (auto arg : arguments) {
        evaluated = evaluate(arg, env);
        evaluatedArgs.push_back(evaluated);
        if (isErrorStorage(evaluated))
            return evaluatedArgs;
    }

    return evaluatedArgs;
}

Storage* invoke(Storage* invocation, std::vector<Storage*> args) {
    if (auto castedInvocation = dynamic_cast<FunctionStorage*>(invocation)) {
        auto scope = new Environment();
        scope->setOutsideScope(castedInvocation->env);

        for (int i = 0; i < castedInvocation->arguments.size(); i++) {
            scope->set(castedInvocation->arguments[i]->value, args[i]);
        }

        if (!castedInvocation->code->hasCode())
            return new ErrorStorage("Can't invoke functions with empty bodies");
        auto invocationResult = evaluate(castedInvocation->code, scope);

        if (auto returnedResult =
                dynamic_cast<ReturnStorage*>(invocationResult)) {
            return returnedResult->value;
        }

        return invocationResult;
    }

    return createError(
        "An invocation was executed on an element which is not a function");
}

Storage* evaluate(Node* node, Environment* env) {
    if (checkBase(node, typeid(Program))) {
        auto program = dynamic_cast<Program*>(node);
        return evaluateProgramStatements(program->statements, env);
    }

    else if (checkBase(node, typeid(ExpressionStatement))) {
        auto statement = dynamic_cast<ExpressionStatement*>(node);
        return evaluate(statement->expression, env);
    }

    else if (checkBase(node, typeid(Integer))) {
        auto integer = dynamic_cast<Integer*>(node);
        return new IntegerStorage(integer->value);
    }

    else if (checkBase(node, typeid(Boolean))) {
        auto boolean = dynamic_cast<Boolean*>(node);
        return boolean->value ? trueStorage : falseStorage;
    }

    else if (checkBase(node, typeid(Prefix))) {
        auto prefix = dynamic_cast<Prefix*>(node);
        auto rightExpression = evaluate(prefix->right, env);
        return evaluatePrefix(prefix->op, rightExpression);
    }

    else if (checkBase(node, typeid(Infix))) {
        auto infix = dynamic_cast<Infix*>(node);
        auto leftExpression = evaluate(infix->left, env);
        if (isErrorStorage(leftExpression))
            return leftExpression;
        auto rightExpression = evaluate(infix->right, env);
        if (isErrorStorage(rightExpression))
            return rightExpression;
        return evaluateInfix(infix->op, leftExpression, rightExpression);
    }

    else if (checkBase(node, typeid(BlockStatement))) {
        auto block = dynamic_cast<BlockStatement*>(node);
        return evaluateBlockStatement(block->statements, env);
    }

    else if (checkBase(node, typeid(Conditional))) {
        auto conditional = dynamic_cast<Conditional*>(node);
        return evaluateIf(conditional, env);
    }

    else if (checkBase(node, typeid(ReturnStatement))) {
        auto statement = dynamic_cast<ReturnStatement*>(node);
        auto result = evaluate(statement->returnValue, env);
        return new ReturnStorage(result);
    }

    else if (checkBase(node, typeid(LetStatement))) {
        auto let = dynamic_cast<LetStatement*>(node);
        auto value = evaluate(let->value, env);
        if (isErrorStorage(value))
            return value;

        // identifier as key
        env->set(let->name->value, value);
        return value;
    }

    else if (checkBase(node, typeid(Identifier))) {
        auto ident = dynamic_cast<Identifier*>(node);
        return env->get(ident->value);
    }

    else if (checkBase(node, typeid(Function))) {
        auto func = dynamic_cast<Function*>(node);
        if (!func->code->hasCode()) {
            return new ErrorStorage(
                "Functions with empty bodies are not allowed");
        }
        return new FunctionStorage(func->arguments, func->code, env);
    }

    else if (checkBase(node, typeid(Invocation))) {
        auto invoc = dynamic_cast<Invocation*>(node);
        auto evaluatedInvoc = evaluate(invoc->function, env);
        if (isErrorStorage(evaluatedInvoc))
            return evaluatedInvoc;

        auto arguments = evaluateArgs(invoc->arguments, env);
        auto argsSize = arguments.size();
        if (argsSize > 0 && isErrorStorage(arguments.at(argsSize - 1))) {
            return arguments[argsSize - 1];
        }

        return invoke(evaluatedInvoc, arguments);
    }

    else if (checkBase(node, typeid(String))) {
        auto str = dynamic_cast<String*>(node);
        return new StringStorage(str->value);
    }

    else if (checkBase(node, typeid(Assignment))) {
        auto assignment = dynamic_cast<Assignment*>(node);
        auto assignedStorage = evaluate(assignment->expression, env);
        auto fetchedStorage = env->get(assignment->identifier->value);

        if (auto castedStorage =
                dynamic_cast<ReferenceStorage*>(fetchedStorage)) {
            auto result = castedStorage->setValue(assignedStorage);
            if (!result)
                return new ErrorStorage(
                    "Cannot assign value with missmatching type");
        } else {
            return env->set(assignment->identifier->value, assignedStorage);
        }

        return fetchedStorage;
    }

    else if (checkBase(node, typeid(Reference))) {
        auto reference = dynamic_cast<Reference*>(node);
        return new ReferenceStorage(reference->referencedIdentifier, env);
    }

    return createError("No implementation found for this functionality");
}

Storage* evaluateProgramStatements(std::vector<Statement*> statements,
                                   Environment* env) {
    Storage* result;

    for (auto statement : statements) {
        result = evaluate(statement, env);

        if (checkBase(result, typeid(ReturnStorage))) {
            auto returnVal = dynamic_cast<ReturnStorage*>(result);
            return returnVal->value;
        } else if (checkBase(result, typeid(ErrorStorage))) {
            return result;
        }
    }

    return result;
}
