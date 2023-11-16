#include "storage.h"
#include <sstream>

Environment::Environment() {}

Storage* Environment::get(const std::string& k) {
    auto it = store.find(k);
    if (it != store.end()) {
        return it->second;
    } else {
        if (outsideScope) {
            it = outsideScope->store.find(k);
            if (it != outsideScope->store.end()) {
                return it->second;
            }
        }

        return new ErrorStorage(k + " is undefined");
    }
}

Storage* Environment::set(const std::string& k, Storage* v) {
    store[k] = v;
    return v;
}

bool Storage::setValue(Storage* storage) { return false; }

void Environment::setOutsideScope(Environment* env) {
    this->outsideScope = env;
}

IntegerStorage::IntegerStorage(int64_t value) : value(value) {}

StorageType IntegerStorage::getType() const { return StorageType::INTEGER; }

std::string IntegerStorage::evaluate() const { return std::to_string(value); }

bool IntegerStorage::setValue(Storage* storage) {
    if (getType() != storage->getType()) {
        return false;
    }

    auto cast = dynamic_cast<IntegerStorage*>(storage);
    this->value = cast->value;

    return true;
}

BooleanStorage::BooleanStorage(bool value) : value(value) {}

StorageType BooleanStorage::getType() const { return StorageType::BOOLEAN; }

std::string BooleanStorage::evaluate() const {
    return value ? "true" : "false";
}

bool BooleanStorage::setValue(Storage* storage) {
    if (getType() != storage->getType()) {
        return false;
    }

    auto cast = dynamic_cast<BooleanStorage*>(storage);
    this->value = cast->value;

    return true;
}

NilStorage::NilStorage() {}

StorageType NilStorage::getType() const { return StorageType::NIL; }

std::string NilStorage::evaluate() const { return "nil"; }

bool NilStorage::setValue(Storage* storage) { return false; }

ReturnStorage::ReturnStorage(Storage* value) : value(value){};

StorageType ReturnStorage::getType() const { return StorageType::RETURN; }

std::string ReturnStorage::evaluate() const { return value->evaluate(); }

bool ReturnStorage::setValue(Storage* storage) { return false; }

ErrorStorage::ErrorStorage(std::string message) {
    const std::string ERROR_PROMPT = "[ERROR]: ";
    this->message = ERROR_PROMPT + message;
};

StorageType ErrorStorage::getType() const { return StorageType::ERROR; }

std::string ErrorStorage::evaluate() const { return message; }

bool ErrorStorage::setValue(Storage* storage) { return false; }

std::unordered_map<StorageType, std::string> storageTypeMap = {
    {StorageType::INTEGER, "INTEGER"}, {StorageType::BOOLEAN, "BOOLEAN"},
    {StorageType::NIL, "NIL"},         {StorageType::RETURN, "RETURN"},
    {StorageType::ERROR, "ERROR"},     {StorageType::FUNCTION, "FUNCTION"},
    {StorageType::STRING, "STRING"}};

std::string parseStorageTypeToString(StorageType sT) {
    auto it = storageTypeMap.find(sT);
    if (it != storageTypeMap.end()) {
        return it->second;
    } else {
        return "UNKNOWN";
    }
}

FunctionStorage::FunctionStorage(std::vector<Identifier*> arguments,
                                 BlockStatement* code, Environment* env)
    : arguments(arguments), code(code), env(env) {}

StorageType FunctionStorage::getType() const { return StorageType::FUNCTION; }

bool FunctionStorage::setValue(Storage* storage) {
    if (getType() != storage->getType()) {
        return false;
    }

    auto cast = dynamic_cast<FunctionStorage*>(storage);
    this->env = cast->env;
    this->arguments = cast->arguments;
    this->code = cast->code;

    return true;
}

std::string FunctionStorage::evaluate() const {
    std::string result = "[function]:\n    arguments: [";

    auto it = arguments.begin();
    while (it != arguments.end()) {
        result += (*it)->toString();
        if (++it != arguments.end()) {
            result += ", ";
        }
    }

    std::ostringstream addressStream;
    addressStream << "0x" << std::hex << reinterpret_cast<uintptr_t>(this);
    result += "]\n    location: \"" + addressStream.str() + "\"";
    return result;
}

StringStorage::StringStorage(std::string value) : value(value) {}

std::string StringStorage::evaluate() const { return value; }

bool StringStorage::setValue(Storage* storage) {
    if (getType() != storage->getType()) {
        return false;
    }

    auto cast = dynamic_cast<StringStorage*>(storage);
    this->value = cast->value;

    return true;
}

StorageType StringStorage::getType() const { return StorageType::STRING; }

ReferenceStorage::ReferenceStorage(std::string reference, Environment* env)
    : reference(reference), environment(env) {}

std::string ReferenceStorage::evaluate() const {
    return environment->get(reference)->evaluate();
}

StorageType ReferenceStorage::getType() const { return StorageType::REFERENCE; }

bool ReferenceStorage::setValue(Storage* storage) {
    return environment->get(reference)->setValue(storage);
}
