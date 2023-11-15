// environment.h
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "storage.h"
#include <string>
#include <unordered_map>

class Environment {
  public:
    Environment();
    Storage* get(const std::string& k);
    Storage* set(const std::string& k, Storage* v);

  private:
    std::unordered_map<std::string, Storage*> store;
};

#endif
