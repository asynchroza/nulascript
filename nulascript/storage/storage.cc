#include "storage.h"

IntegerStorage::IntegerStorage(int64_t value) : value(value) {}

StorageType IntegerStorage::getType() const { return StorageType::INTEGER; }

std::string IntegerStorage::evaluate() const { return std::to_string(value); }

BooleanStorage::BooleanStorage(bool value) : value(value) {}

StorageType BooleanStorage::getType() const { return StorageType::BOOLEAN; }

std::string BooleanStorage::evaluate() const {
    return value ? "true" : "false";
}

NilStorage::NilStorage() {}

StorageType NilStorage::getType() const { return StorageType::NIL; }

std::string NilStorage::evaluate() const { return "nil"; }

ReturnStorage::ReturnStorage(Storage* value) : value(value){};

StorageType ReturnStorage::getType() const { return StorageType::RETURN; }

std::string ReturnStorage::evaluate() const { return value->evaluate(); }