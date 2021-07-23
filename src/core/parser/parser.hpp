#pragma once

#include "../../utils/format.hpp"
#include "../../utils/operator.hpp"

#include "../builtin/builtinDefinitions.hpp"
#include "../../defs/expressions.hpp"

#include "../lexer/lexer.hpp"

#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

using namespace Expressions;

class Parser {
    private:
        std::vector<Token> tokenStream;

        unsigned int currentTokenIndex = 0;
        bool error = false;
        
        // Token parsing/Tree making
        std::shared_ptr<Program> parseProgram();
        void preprocessImports();
        std::vector<Token> parseImport(int tokenIndex);
        ExpPtr parseExpression();
        ExpPtr parseSimpleExpression();
        std::shared_ptr<Typeclass> parseTypeclass();
        std::shared_ptr<Branch> parseBranch();
        std::shared_ptr<ListDefinition> parseList();
        std::shared_ptr<TupleDefinition> parseTuple();
        std::shared_ptr<Match> parseMatch();
        std::shared_ptr<Case> parseCase();
        ExpPtr parseUtight(int min);
        ExpPtr parseUtight();
        ExpPtr parseTight();
        std::shared_ptr<Application> parseApplication();
        std::shared_ptr<Function> parseFunc();
        std::shared_ptr<Argument> parseArg(const std::vector<Types::GenTypePtr> & gens);
        ExpPtr parseAtom();
        Types::TypePtr parseType(const std::vector<Types::GenTypePtr> & gens);

        // Helpers

        void advance() { currentTokenIndex++; }
        Token currentToken() { return tokenStream.at(currentTokenIndex); }
        bool inBounds() { return currentTokenIndex < tokenStream.size(); }

        bool match(const Token::TokenType tokenType, const std::string & text);
        bool matchNoAdvance(const Token::TokenType tokenType, const std::string & text);
        void skip(const std::string & text);

        std::string dummy();
        bool isValue(const std::string & valueString);
        char getEscapedCharacter(const std::string & escapeSequence);

        void printError(bool useUnexpected, const std::string & errorString, const std::string & expected = "$");

    public:
        explicit Parser(const std::vector<Token> & tokenStream);

        ExpPtr makeTree();
        bool errorOccurred() const { return error; }
};