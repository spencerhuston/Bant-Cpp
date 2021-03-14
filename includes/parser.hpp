#pragma once

#include "utils/format.hpp"
#include "utils/operator.hpp"
#include "defs/expressions.hpp"

#include <sstream>
#include <string>
#include <vector>
#include <memory>

using namespace Expressions;

class Parser {
    private:
        const std::vector<Token> tokenStream;
        unsigned int currentTokenIndex = 0;
        bool error = false;
        
        // Token parsing/Tree making
        Expression parseProgram();
        Expression parseExpression();
		Expression parseSimpleExpression();
        Expression parseBranch();
        Expression parseList();
        Expression parseMatch();
        Case parseCase();
		Expression parseUtight(int min);
		Expression parseUtight();
		Expression parseTight();
		Expression parseFunc();
		Expression parseArg(const std::vector<Types::GenType> & gens);
		Expression parseAtom();
		Types::Type parseType(const std::vector<Types::GenType> & gens);

        // Helpers

        // Token/Stream handling
        void advance() { currentTokenIndex++; }
        Token currentToken() { return tokenStream.at(currentTokenIndex); }
        bool inBounds() { return currentTokenIndex < tokenStream.size(); }

        bool match(const Token::TokenType tokenType, const std::string text);
        Token peek();
        void skip(const std::string & text);

        std::string dummy();

        void printError(const std::string & errorString);

    public:
        Parser(const std::vector<Token> & tokenStream);
        Expression makeTree();
};