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
        BlockGet parseBlockGet();
        Application parseApplication();
        Expression parseFunc();
        Expression parseArg(const std::vector<Types::GenType> & gens);
        Expression parseAtom();
        Types::Type parseType(const std::vector<Types::GenType> & gens);

        // Helpers

        void advance() { currentTokenIndex++; }
        Token currentToken() { return tokenStream.at(currentTokenIndex); }
        bool inBounds() { return currentTokenIndex < tokenStream.size(); }

        bool match(const Token::TokenType tokenType, const std::string text);
        bool matchNoAdvance(const Token::TokenType tokenType, const std::string text);
        Token peek();
        void skip(const std::string & text);

        std::string dummy();
        bool isValue(const std::string & valueString);
        char getEscapedCharacter(const std::string & escapeSequence);

        void printError(const std::string & errorString);

    public:
        Parser(const std::vector<Token> & tokenStream);

        Expression makeTree();
        bool errorOccurred() const { return error; }
};