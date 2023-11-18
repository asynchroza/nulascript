#ifndef REPL_H
#define REPL_H

#include <string>

class Interpreter {
  public:
    static void interpret(const std::string& filename);

  private:
    static const std::string PROMPT;
};

#endif