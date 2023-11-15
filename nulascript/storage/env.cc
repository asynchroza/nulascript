#include "env.h"

Environment::Environment() {}

Storage* Environment::get(const std::string& k) {
    auto it = store.find(k);
    if (it != store.end()) {
        return it->second;
    } else {
        // TODO: think of better implementation
        return new ErrorStorage("Couldn't find value");
    }
}

Storage* Environment::set(const std::string& k, Storage* v) {
    store[k] = v;
    return v;
}
