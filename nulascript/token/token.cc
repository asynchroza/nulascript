#include "token.h"

TokenLookup::TokenLookup() {
    keywords = {{"func", FUNC},     {"def", LET}, {"true", TRUE},
                {"false", FALSE},   {"if", IF},   {"else", ELSE},
                {"return", RETURN}, {"is", IS},   {"not", BANG_OR_NOT},
                {"for", FOR}};
}

TokenType TokenLookup::lookupIdent(const std::string& ident) {
    auto it = keywords.find(ident);
    if (it != keywords.end()) {
        return it->second;
    }

    return IDENT;
}
