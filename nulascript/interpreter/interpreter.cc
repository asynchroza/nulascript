#include "interpreter.h"
#include "eval.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include <fstream>
#include <iostream>

void Interpreter::interpret(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::string code;

    while (std::getline(file, line)) {
        code += line + "\n";
    }

    auto environment = new Environment();

    Lexer l(code);
    Parser p(l);
    Program* program = p.parseProgram();

    if (p.getErrors().size() != 0) {
        for (auto msg : p.getErrors()) {
            std::cout << msg << std::endl;
        }
        return;
    }

    auto resolved = evaluate(program, environment);

    if (resolved) {
        if (resolved->getType() == StorageType::NIL) {
            std::cout << "undefined"
                      << "\n";
        } else if (resolved->getType() == StorageType::EMPTY) {
            std::cout << "\n";
        } else if (resolved->getType() == StorageType::ERROR) {
            std::cout << resolved->evaluate() << "\n";
        }
    }
}
