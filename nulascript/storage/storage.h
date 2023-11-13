// storage.h
#pragma once
#ifndef STORAGE_H
#define STORAGE_H

#include <iostream>
#include <string>

enum class StorageType { INTEGER, BOOLEAN, NIL };

class Storage {
  public:
    virtual StorageType getType() const = 0;
    virtual std::string evaluate() const = 0;
};

class IntegerStorage : public Storage {
  public:
    int64_t value;

    // Constructor
    IntegerStorage(int64_t value);

    // Overrides
    StorageType getType() const override;
    std::string evaluate() const override;
};

class BooleanStorage : public Storage {
  public:
    bool value;

    // Constructor
    BooleanStorage(bool value);

    // Overrides
    StorageType getType() const override;
    std::string evaluate() const override;
};

class NilStorage : public Storage {
  public:
    // Constructor
    NilStorage();

    // Overrides
    StorageType getType() const override;
    std::string evaluate() const override;
};

#endif // STORAGE_H
