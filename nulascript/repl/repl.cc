#include <iostream>
#include "repl.h"
#include "token.h"
#include "lexer.h"

const std::string REPL::PROMPT = "> ";

void REPL::start() {
    std::string line;

    while(true) {
        std::cout << REPL::PROMPT;
        if (!std::getline(std::cin, line)) {
            break; // Exit the loop on EOF or error
        }

        Lexer l (line);
        for (Token token = l.getNextToken(); token.type != TokenType::EOF_TYPE; token = l.getNextToken()) {
            std::cout << token.literal << std::endl;
        }
    }
}