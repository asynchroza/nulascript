#include "token.h"

TokenLookup::TokenLookup() {
    keywords = {
        {"fn", FUNC},
        {"let", LET},
        {"true", TRUE},
        {"false", FALSE},
        {"if", IF},
        {"else", ELSE},
        {"return", RETURN},
        {"eq", EQUAL},
        {"not", NOT_EQUAL}
    };
}

TokenType TokenLookup::lookupIdent(const std::string& ident) {
    auto it = keywords.find(ident);
    if (it != keywords.end()) {
        return it->second;
    }

    return IDENT;
}
