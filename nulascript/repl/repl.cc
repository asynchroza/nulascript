#include "repl.h"
#include "eval.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include <iostream>

const std::string REPL::PROMPT = "> ";

void REPL::start() {
    std::string line;

    auto environment = new Environment();
    // initialize std
    auto std = new STD(environment);
    environment->set("lib", std);

    while (true) {
        std::cout << REPL::PROMPT;
        if (!std::getline(std::cin, line)) {
            break; // Exit the loop on EOF or error
        }

        Lexer l(line);
        Parser p(l);
        Program* program = p.parseProgram();

        if (p.getErrors().size() != 0) {
            for (auto msg : p.getErrors()) {
                std::cout << msg << std::endl;
            }
            continue;
        }

        auto resolved = evaluate(program, environment);

        if (resolved) {
            if (resolved->getType() == StorageType::NIL) {
                std::cout << "undefined"
                          << "\n\n";
            } else {
                std::cout << resolved->evaluate() << "\n\n";
            }
        }
    }
}