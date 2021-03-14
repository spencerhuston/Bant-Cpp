#include "../includes/parser.hpp"

Parser::Parser(const std::vector<Token> & tokenStream)
: tokenStream(tokenStream) { }

Expression
Parser::makeTree() {
    Expression tree = parseProgram();
    // err if anything leftover
    return tree;
}

Expression
Parser::parseProgram() {
    std::vector<Expression> functions;
    while (match(Token::TokenType::KEYWORD, "func"))
        functions.push_back(parseFunc());

    Expression body = parseExpression();
    return Program{currentToken(), functions, body};
}

Expression
Parser::parseExpression() {
    if (!inBounds())
        return Expression::End();

    if (match(Token::TokenType::KEYWORD, "val")) {
        const std::string ident = currentToken().text;
        Token token = currentToken();
        advance();

        skip(":");
        Types::Type valueType = parseType({});
        skip("=");
        Expression valueExpression = parseSimpleExpression();
        skip(";");
        Expression afterExpression = parseExpression();
        return Let{token, ident, valueType, valueExpression, afterExpression};
    } else {
        Token token = currentToken();
        Expression simpleExpression = parseSimpleExpression();
        if (match(Token::TokenType::DELIM, ";")) {
            Expression expression = parseExpression();
            return Let{token, dummy(), Types::NullType(), simpleExpression, expression};
        }
        return simpleExpression;
    }
}

Expression
Parser::parseSimpleExpression() {
    if (match(Token::TokenType::KEYWORD, "if")) {
        return parseBranch();
    } else if (match(Token::TokenType::KEYWORD, "new")) {
        return parseList();
    } else if (match(Token::TokenType::KEYWORD, "match")) {
        return parseMatch();
    } else if (match(Token::TokenType::KEYWORD, "func")) {
        return parseProgram();
    }
    return parseUtight(0);
}

Expression
Parser::parseBranch() {
    const Token token = currentToken();
    skip("(");
    Expression condition = parseSimpleExpression();
    skip(")");

    Expression trueBranch = parseSimpleExpression();
    if (match(Token::TokenType::KEYWORD, "else")) {
        return Branch{token, condition, trueBranch, parseSimpleExpression()};
    }
    return Branch{token, condition, trueBranch, Literal{token, Types::NullType()}};
}

Expression
Parser::parseList() {
    const Token token = currentToken();
    skip("List");
	skip("{");

    std::vector<Expression> listValues;
    if (inBounds() && currentToken().text != "}") {
        listValues.push_back(parseSimpleExpression());
        while (match(Token::TokenType::DELIM, ",")) {
            listValues.push_back(parseSimpleExpression());
        }
    }

    skip("}");
    return ListDefinition{token, listValues};
}

Expression
Parser::parseMatch() {
    const Token token = currentToken();
    skip("(");
    const std::string ident = currentToken().text;
    advance();
    skip(")");
	skip("{");

    std::vector<Case> cases;
    while (match(Token::TokenType::KEYWORD, "case")) {
        cases.push_back(parseCase());
    }
    skip("}");
    return Match{token, ident, cases};
}

Case
Parser::parseCase() {
    const Token token = currentToken();
    std::unique_ptr<Expression> ident; // making this a pointer avoids calling private constructor to init
    if (match(Token::TokenType::KEYWORD, "any")) {
        ident = std::make_unique<Reference>(token, Types::NullType(), std::string("$any"));
    } else {
        ident = std::make_unique<Expression>(parseAtom());
    }
    skip("=");
    skip("{");
    Expression block = parseSimpleExpression();
    skip("}");
    skip(";");
    return Case{token, *ident, block};
}

Expression
Parser::parseUtight(int min) {
    
}

Expression
Parser::parseUtight() {
    
}

Expression
Parser::parseTight() {
    
}

Expression
Parser::parseFunc() {
    
}

Expression
Parser::parseArg(const std::vector<Types::GenType> & gens) {
    
}

Expression
Parser::parseAtom() {
    
}

Types::Type
Parser::parseType(const std::vector<Types::GenType> & gens) {
    
}

bool
Parser::match(const Token::TokenType tokenType, const std::string text) {
    if (inBounds() && currentToken().type == tokenType && currentToken().text == text) {
        advance();
        return true;
    }
    return false;
}

Token
Parser::peek() {
    if (currentTokenIndex == tokenStream.size() - 1) {
        return Token(Token::TokenType::DELIM,
                     FilePosition(-1, -1, "END"),
                     std::string({}));
    }

    return tokenStream.at(currentTokenIndex + 1);
}

void
Parser::skip(const std::string & text) {
    if (inBounds() && currentToken().text != text) {
        printError(text);
    }
    advance();
}

std::string
Parser::dummy() {
    static int dummy_count = 0;
	return ("dummy$" + std::to_string(dummy_count++));
}

void
Parser::printError(const std::string & errorString) {
    error = true;
    FilePosition position = currentToken().position;
    std::stringstream errorStream;
    errorStream << "Line: " << position.fileLine
                << ", Column: " << position.fileColumn - 1 << std::endl
                << "Unexpected character: " << errorString << std::endl << std::endl
                << position.currentLineText << std::endl
                << std::string(position.fileColumn - errorString.length() - 1, ' ') << "^";
    Format::printError(errorStream.str());
}