#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "ast.h"
#include "storage.h"

Storage* evaluate(Node* node, Environment* env);

#endif // EVALUATOR_H
