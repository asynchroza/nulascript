#include "eval.h"

BooleanStorage* trueStorage = new BooleanStorage(true);
BooleanStorage* falseStorage = new BooleanStorage(false);
NilStorage* nilStorage = new NilStorage();
EmptyStorage* emptyStorage = new EmptyStorage();

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

Storage* evaluatePointerExpression(Storage* rightExpression) {
    auto ref = dynamic_cast<ReferenceStorage*>(rightExpression);
    return ref->environment->get(ref->reference);
}

Storage* evaluatePrefix(std::string op, Storage* rightExpression) {
    if (op == "!" || op == "not") {
        return evaluateNotExpression(rightExpression);
    } else if (op == "-") {
        return evaluateMinusExpression(rightExpression);
    } else if (op == "*") {
        return evaluatePointerExpression(rightExpression);
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
    if (auto referencedInvocation =
            dynamic_cast<ReferenceStorage*>(invocation)) {
        invocation = referencedInvocation->environment->get(
            referencedInvocation->reference);
    }

    if (auto defaultInvocation = dynamic_cast<StandardFunction*>(invocation)) {
        return defaultInvocation->function(args);
    }

    // ????
    // Consider extracting this code into a function and swapping its position
    // with the code above. This will help eliminate redundant casting to
    // ReferenceStorage, as it is anticipated that such occurrences will be less
    // frequent than regular invocations.
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

// TODO: Move them elsewhere
// STANDARD FUNCTION DEFINITIONS

Storage* printStorage(std::vector<Storage*> args) {
    for (auto arg : args) {
        std::cout << arg->evaluate() << " ";
    }

    std::cout << "\n";

    return emptyStorage;
}

// TODO: Deprecate after implementing actual loops
Storage* runLoop(std::vector<Storage*> args) {
    auto len = dynamic_cast<IntegerStorage*>(args[0]);
    auto func = dynamic_cast<FunctionStorage*>(args[1]);

    if (!len && !func) {
        return new ErrorStorage("Provided arguments do not match required "
                                "arguments - int & function");
    }

    for (int i = 0; i < len->value; i++) {
        invoke(func, std::vector<Storage*>());
    }

    return emptyStorage;
}

bool evaluateConditionalExpression(int64_t val, std::string op,
                                   int64_t threshold) {
    if (op == "is" || op == "==") {
        return val == threshold;
    } else if (op == ">") {
        return val > threshold;
    } else if (op == "<") {
        return val < threshold;
    } else if (op == ">=") {
        return val >= threshold;
    } else if (op == "<=") {
        return val <= threshold;
    }

    // ! Consider using else instead of having this path
    return false;
}

int64_t getValueBasedOnOperator(int64_t val, std::string op,
                                int64_t increment) {
    if (op == "+") {
        return val + increment;
    } else if (op == "-") {
        return val - increment;
    } else if (op == "*") {
        return val * increment;
    } else if (op == "/") {
        return val / increment;
    }
}

Storage* runForLoop(ForLoop* fl, Environment* env) {
    auto expression = evaluate(fl->definition.variable, env);
    IntegerStorage* variable;
    bool shouldReference = false;

    if (checkBase(expression, typeid(ReferenceStorage))) {
        variable = dynamic_cast<IntegerStorage*>(
            env->get(dynamic_cast<ReferenceStorage*>(expression)->reference));
        shouldReference = true;
    } else {
        variable = dynamic_cast<IntegerStorage*>(expression);
        if (!variable) {
            return new ErrorStorage(
                "[LOOP] Incorrectly provisioned initialization variable");
        }
    }

    if (!fl->code->hasCode()) {
        return new ErrorStorage("[LOOP] Doesn't have body");
    }

    Infix* increment = dynamic_cast<Infix*>(fl->definition.increment);
    if (!increment) {
        return new ErrorStorage(
            "[LOOP] Incorrectly provisioned incremental expression");
    }

    Infix* conditional = dynamic_cast<Infix*>(fl->definition.conditional);

    if (!conditional) {
        return new ErrorStorage(
            "[LOOP] Incorrectly provisioned conditional statement");
    }

    // variable identifier -> for (def i = 5; i < 10; -> i + 1)
    Identifier* incrementalIdent = dynamic_cast<Identifier*>(increment->left);
    if (!incrementalIdent) {
        return new ErrorStorage("[LOOP] Provisioned variable identifier in "
                                "incremental expression is incorrect");
    }

    // TODO: make it work with identifiers
    Integer* step = dynamic_cast<Integer*>(increment->right);
    if (!step) {
        return new ErrorStorage(
            "[LOOP] Right side of incremental expression is not an integer");
    }

    // variable identifier -> for (def i = 5; -> i < 10; i + 1)
    Identifier* identifier = dynamic_cast<Identifier*>(conditional->left);
    if (!identifier) {
        return new ErrorStorage("[LOOP] Provisioned variable identifier in "
                                "conditional expression is incorrect");
    }

    Integer* threshold = dynamic_cast<Integer*>(conditional->right);
    if (!threshold) {
        return new ErrorStorage(
            "[LOOP] Right side of conditional expression is not an integer");
    }

    IntegerStorage* initializer;
    initializer =
        dynamic_cast<IntegerStorage*>(env->get(identifier->token.literal));

    if (!initializer) {
        std::string errMsg =
            "[LOOP] Provisioned initialization value is not of type integer";
        if (!checkBase(env->get(identifier->token.literal),
                       typeid(ReferenceStorage))) {
            return new ErrorStorage(errMsg);
        }

        initializer = dynamic_cast<IntegerStorage*>(env->get(
            dynamic_cast<ReferenceStorage*>(env->get(identifier->token.literal))
                ->reference));
        if (!initializer) {
            return new ErrorStorage(errMsg);
        }
    }

    // incremental loop
    for (int i = initializer->value; true; i++) {

        // this handles referencing
        auto current =
            dynamic_cast<IntegerStorage*>(env->get(identifier->token.literal));
        if (!current) {
            current = dynamic_cast<IntegerStorage*>(
                env->get(dynamic_cast<ReferenceStorage*>(
                             env->get(identifier->token.literal))
                             ->reference));

            // do not run this check if it's been done once
            if (!current && i != initializer->value) {
                return new ErrorStorage("[LOOP] Current value is neither a "
                                        "reference nor an integer");
            }
        }

        if (!evaluateConditionalExpression(current->value, conditional->op,
                                           threshold->value))
            break;

        for (auto stmt : fl->code->statements) {
            evaluate(stmt, env);
        }

        // reflect increase in environment
        if (shouldReference) {
            auto reference = env->get(identifier->value);
            auto loopVariable = dynamic_cast<IntegerStorage*>(env->get(
                dynamic_cast<ReferenceStorage*>(reference)->reference));

            loopVariable->value = getValueBasedOnOperator(
                loopVariable->value, increment->op, step->value);

            env->set(identifier->value, reference);

        } else {
            auto loopVariable =
                dynamic_cast<IntegerStorage*>(env->get(identifier->value));

            int64_t increasedValue = getValueBasedOnOperator(
                loopVariable->value, increment->op, step->value);

            env->set(identifier->value, new IntegerStorage(increasedValue));
        }
    }

    env->remove(identifier->value);
    return emptyStorage;
}

Storage* loggingFunction(std::vector<Storage*> args) {
    return printStorage(args);
}

std::unordered_map<std::string, Storage*> standardFunctions = {
    {"log", new StandardFunction(&loggingFunction)},
    {"loop", new StandardFunction([](std::vector<Storage*> args) -> Storage* {
         return runLoop(args);
     })}};

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
        auto fetched = env->get(ident->value);

        if (dynamic_cast<ErrorStorage*>(fetched)) {
            auto it = standardFunctions.find(ident->value);

            if (it != standardFunctions.end()) {
                return it->second;
            }
        }

        // return default error object if nothing is found
        return fetched;
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
            return env->set(castedStorage->reference, assignedStorage);
        }

        return env->set(assignment->identifier->value, assignedStorage);
    }

    else if (checkBase(node, typeid(Reference))) {
        auto reference = dynamic_cast<Reference*>(node);
        return new ReferenceStorage(reference->referencedIdentifier, env);
    }

    else if (checkBase(node, typeid(Pointer))) {
        auto pointer = dynamic_cast<Pointer*>(node);
        return env->get(pointer->dereferencedIdentifier);
    }

    else if (checkBase(node, typeid(ForLoop))) {
        auto fl = dynamic_cast<ForLoop*>(node);
        return runForLoop(fl, env);
    }

    else if (checkBase(node, typeid(Comment))) {
        return emptyStorage;
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
