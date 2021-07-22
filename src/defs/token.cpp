#include "token.hpp"

Token::Token(const TokenType tokenType, const FilePosition & filePosition, const std::string & tokenText) 
: type(tokenType), position(filePosition), text(tokenText) {

}

const std::string
Token::toString() const {
    std::stringstream tokenString;
    tokenString << getTokenTypeAsString(type) << std::endl
                << std::to_string(position.fileLine) << ", " << std::to_string(position.fileColumn) << std::endl
                << text << std::endl;
    return tokenString.str();
}

const std::string
Token::getTokenTypeAsString(const TokenType type) {
    switch (type) {
        case TokenType::DELIM:
            return "DELIM";
            break;
        case TokenType::KEYWORD:
            return "KEYWORD";
            break;
        case TokenType::VAL:
            return "VAL";
            break;
        case TokenType::IDENT:
            return "IDENT";
            break;
        case TokenType::ERROR:
            return "ERROR";
            break;
        default:
            std::cout << "Unknown token type" << std::endl;
    }
    return "Unknown token type";
}