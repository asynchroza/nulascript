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

ErrorStorage::ErrorStorage(std::string message) {
    const std::string ERROR_PROMPT = "[ERROR]: ";
    this->message = ERROR_PROMPT + message;
};

StorageType ErrorStorage::getType() const { return StorageType::ERROR; }

std::string ErrorStorage::evaluate() const { return message; }

std::unordered_map<StorageType, std::string> storageTypeMap = {
    {StorageType::INTEGER, "INTEGER"},
    {StorageType::BOOLEAN, "BOOLEAN"},
    {StorageType::NIL, "NIL"},
    {StorageType::RETURN, "RETURN"},
    {StorageType::ERROR, "ERROR"}};

std::string parseStorageTypeToString(StorageType sT) {
    auto it = storageTypeMap.find(sT);
    if (it != storageTypeMap.end()) {
        return it->second;
    } else {
        return "UNKNOWN";
    }
}