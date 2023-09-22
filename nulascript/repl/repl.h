#ifndef REPL_H
#define REPL_H

#include <string>

class REPL {
public:
    static void start();

private:
    static const std::string PROMPT;
};

#endif