// storage.h
#pragma once
#ifndef STORAGE_H
#define STORAGE_H

#include <iostream>
#include <string>

enum class StorageType { INTEGER, BOOLEAN, NIL, RETURN };

class Storage {
  public:
    virtual StorageType getType() const = 0;
    virtual std::string evaluate() const = 0;
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

#endif // STORAGE_H
