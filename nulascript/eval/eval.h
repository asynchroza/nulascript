#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "ast.h"
#include "storage.h"

// Function declarations
Storage* evaluate(Node* node);
Storage* evaluateSequence(std::vector<Statement*> statements);

#endif // EVALUATOR_H
