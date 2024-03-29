// storage.h
#pragma once
#ifndef STORAGE_H
#define STORAGE_H

#include "ast.h"
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

enum class StorageType {
    INTEGER,
    BOOLEAN,
    NIL,
    RETURN,
    ERROR,
    FUNCTION,
    STRING,
    REFERENCE,
    POINTER,
    STANDARD_FUNCTION,
    EMPTY
};

extern std::unordered_map<StorageType, std::string> storageTypeMap;

std::string parseStorageTypeToString(StorageType sT);

class Storage {
  public:
    virtual StorageType getType() const = 0;
    virtual std::string evaluate() const = 0;
};

class EmptyStorage : public Storage {
  public:
    EmptyStorage();
    StorageType getType() const override;
    std::string evaluate() const override;
};

class Environment {
  public:
    Environment();
    Storage* get(const std::string& k);
    Storage* set(const std::string& k, Storage* v);
    void remove(const std::string& k);
    void setOutsideScope(Environment* env);

  private:
    std::unordered_map<std::string, Storage*> store;
    Environment* outsideScope;
};

class IntegerStorage : public Storage {
  public:
    int64_t value;

  public:
    IntegerStorage(int64_t value);

    StorageType getType() const override;
    std::string evaluate() const override;
};

class BooleanStorage : public Storage {
  public:
    bool value;

  public:
    BooleanStorage(bool value);

    StorageType getType() const override;
    std::string evaluate() const override;
};

class NilStorage : public Storage {
  public:
    NilStorage();
    StorageType getType() const override;
    std::string evaluate() const override;
};

class ReturnStorage : public Storage {
  public:
    Storage* value;

  public:
    ReturnStorage(Storage* value);
    StorageType getType() const override;
    std::string evaluate() const override;
};

class ErrorStorage : public Storage {
  public:
    std::string message;

  public:
    ErrorStorage(std::string message);
    StorageType getType() const override;
    std::string evaluate() const override;
};

class FunctionStorage : public Storage {
  public:
    std::vector<Identifier*> arguments;
    BlockStatement* code;
    Environment* env;

  public:
    FunctionStorage(std::vector<Identifier*> arguments, BlockStatement* code,
                    Environment* env);
    StorageType getType() const override;
    std::string evaluate() const override;
};

class StringStorage : public Storage {
  public:
    std::string value;

  public:
    StringStorage(std::string value);
    StorageType getType() const override;
    std::string evaluate() const override;
};

class ReferenceStorage : public Storage {
  public:
    std::string reference;
    Environment* environment;

  public:
    ReferenceStorage(std::string reference, Environment* environment);
    StorageType getType() const override;
    std::string evaluate() const override;
};

using TFunction = std::function<Storage*(std::vector<Storage*>)>;

class StandardFunction : public Storage {
  public:
    TFunction function;

  public:
    StandardFunction(TFunction function);
    StorageType getType() const override;
    std::string evaluate() const override;
};

#endif // STORAGE_H
