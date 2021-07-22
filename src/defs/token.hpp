#pragma once

#include "../utils/filePosition.hpp"

#include <iostream>
#include <sstream>
#include <string>

class Token {
    public:
        enum class TokenType {
            DELIM,
            KEYWORD,
            VAL,
            IDENT,
            ERROR
        };
        
        TokenType type = static_cast<TokenType>(0);
        FilePosition position;
        std::string text{};

        Token(const TokenType tokenType, const FilePosition & filePosition, const std::string & tokenText);
        const std::string toString() const;
        static const std::string getTokenTypeAsString(const TokenType type);
};