#include "interpreter.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    const std::string filename = argv[1];
    Interpreter::interpret(filename);

    return 0;
}