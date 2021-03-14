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
    return Branch{token, condition, trueBranch, Literal{token}};
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
    Expression leftSide = parseUtight();

    while (inBounds() && Operator::isBinaryOperator(currentToken().text, min)) {
        Token token = currentToken();
        Operator::OperatorTypes op = Operator::getOperator(currentToken().text);
        advance();

        int tempMin = Operator::getPrecedence(op) + 1;
        Expression rightSide = parseUtight(tempMin);
        leftSide = Primitive{token, leftSide.returnType, op, leftSide, rightSide};
    }
    return leftSide;
}

Expression
Parser::parseUtight() {
    Operator::OperatorTypes op;
    op = Operator::OperatorTypes::NONE;
    if (match(Token::TokenType::DELIM, "+")) {
		op = Operator::OperatorTypes::PLUS;
	} else if (match(Token::TokenType::DELIM, "-")) {
		op = Operator::OperatorTypes::MINUS;
	} else if (match(Token::TokenType::DELIM, "!")) {
		op = Operator::OperatorTypes::NOT;
	}

    Expression rightSide = parseTight();
    Token token = currentToken();
    if (op == Operator::OperatorTypes::PLUS || op == Operator::OperatorTypes::MINUS) {
		return Primitive{token, Types::IntType(), op, Literal<int>{token, Types::IntType(), 0}, rightSide};
	} else if (op == Operator::OperatorTypes::NOT) {
		return Primitive{token, Types::BoolType(), op, Literal<bool>{token, Types::BoolType(), false}, rightSide};
	}
	return rightSide;
}

Expression
Parser::parseTight() {
    if (match(Token::TokenType::DELIM, "{")) {
        Expression exp = parseExpression();
        skip("}");
        return exp;
    } else if (peek().type == Token::TokenType::DELIM && peek().text == "[") {
        return parseBlockGet;
    }
    return parseApplication();
}

BlockGet
Parser::parseBlockGet() {
    Token token = currentToken();

    Expression reference = parseAtom();
    skip("[");
	Expression index = parseSimpleExpression();
	skip("]");
    BlockGet blockGet{token, reference, index};
    while (match(Token::TokenType::DELIM, "[")) {
        BlockGet blockGetInner{currentToken(), blockGet.reference, blockGet.index};
        blockGet.reference = blockGetInner;
        blockGet.index = parseSimpleExpression();
        skip("]");
    }

	return blockGet;
}

Application
Parser::parseApplication() {
    Token token = currentToken();

    Expression ident = parseAtom();
    if (match(Token::TokenType::DELIM, "(")) {
        std::vector<Expression> arguments;
        skip("(");
        if (inBounds() && currentToken().text != ")") {
            arguments.push_back(parseSimpleExpression());
        }
        while (match(Token::TokenType::DELIM, ",")) {
            arguments.push_back(parseSimpleExpression());	
        }
        skip(")");

        Application app{token, ident, arguments};

        while (match(Token::TokenType::DELIM, "(")) {
                token = currentToken();

				std::vector<Expression> argumentsInner;
                skip("(");
                if (inBounds() && currentToken().text != ")") {
                    argumentsInner.push_back(parseSimpleExpression());
                }
                while (match(Token::TokenType::DELIM, ",")) {
                    argumentsInner.push_back(parseSimpleExpression());	
                }
				skip(")");
                Application appInner{token, ident, arguments};
                app.functionIdent = appInner;
                app.arguments = argumentsInner;
			}
			return app;
    }
    return ident;
}

Expression
Parser::parseFunc() {
    return Expression::End();
}

Expression
Parser::parseArg(const std::vector<Types::GenType> & genericParameterList) {
    const std::string argumentName = currentToken().text;
    Token token = currentToken();
    advance();
    skip(":");
    Types::Type argumentType = parseType(genericParameterList);
    return Argument{token, argumentType, argumentName};
}

Expression
Parser::parseAtom() {
    if (match(Token::TokenType::DELIM, "(")) {
        Expression exp = parseSimpleExpression();
        skip(")");
        return exp;
    } else if (inBounds()) {
        if (currentToken().type == Token::TokenType::IDENT) {

        } else {
            Token token = currentToken();
            if (matchNoAdvance(Token::TokenType::KEYWORD, "true") ||
                matchNoAdvance(Token::TokenType::KEYWORD, "false")) {
                Literal lit<bool>{currentToken(), Types::BoolType(), (currentToken().text == "true")};
                advance();
                return lit;
            } else if (match(Token::TokenType::KEYWORD, "null")) {
                return Literal{token};
            } else if (isValue(currentToken().text)) {
                Literal lit<int>{currentToken(), Types::BoolType(), std::stoi(currentToken().text)};
                advance();
                return lit;
            } else if (match(Token::TokenType::DELIM, "'") && currentToken().text.length() <= 2) {
                Literal lit<char>{currentToken(), Types::CharType(), getEscapedCharacter(currentToken().text)};
                advance();
                skip("'");
                return lit;
            } else {
                printError(currentToken().text);
            }
        }
    }
    
    Format::printError(std::string("Parsing error: ") + currentToken().toString());
	return Expression::End();
}

Types::Type
Parser::parseType(const std::vector<Types::GenType> & genericParameterList) {
    if (inBounds() && currentToken().type == Token::TokenType::KEYWORD && currentToken().text != "List") {
		const std::string typeString = currentToken().text;
        
		std::unique_ptr<Types::Type> type;
		if (typeString == "int") type = std::make_unique<Types::IntType>();
		else if (typeString == "bool") type = std::make_unique<Types::BoolType>();
		else if (typeString == "char") type = std::make_unique<Types::CharType>();
		else if (typeString == "null") type = std::make_unique<Types::NullType>();
		else {
			Format::printError(std::string("Unexpected type: ") + typeString);
			return Types::NullType();
		}
		advance();
		
		if (match(Token::TokenType::DELIM, "->")) {
            return Types::FuncType{genericParameterList, std::vector({*type}), parseType(genericParameterList)};
		}

		return *type;
	} else if (match(Token::TokenType::KEYWORD, "List")) {
        skip("[");
        Types::Type listDataType = parseType(genericParameterList);
        skip("]");
        return Types::ListType{listDataType};
    } else if (match(Token::TokenType::DELIM, "(")) {
        std::vector<Types::Type> functionArgumentTypes{parseType(genericParameterList)};
        while (match(Token::TokenType::DELIM, ",")) {
			functionArgumentTypes.push_back(parseType(genericParameterList));	
		}
		skip(")");
		skip("->");
		return Types::FuncType{genericParameterList, functionArgumentTypes, parseType(genericParameterList)};
    } else {
        const std::string parameterName = currentToken().text;
		bool genericNameMatches = false;

		std::unique_ptr<Types::Type> type;
		for (const auto genericParameterName : genericParameterList) {
			if (genericParameterName.identifier == parameterName) {
				type = std::make_unique<Types::GenType>(parameterName);
				genericNameMatches = true;
				advance();
				break;
			}
		}

		if (!genericNameMatches) {
            Format::printError(std::string("Undefined generic type: ") + parameterName);
			return Types::NullType();
		}
		return *type;
    }
}

bool
Parser::match(const Token::TokenType tokenType, const std::string text) {
    if (inBounds() && currentToken().type == tokenType && currentToken().text == text) {
        advance();
        return true;
    }
    return false;
}

bool
Parser::matchNoAdvance(const Token::TokenType tokenType, const std::string text) {
    if (inBounds() && currentToken().type == tokenType && currentToken().text == text) {
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

bool
Parser::isValue(const std::string & valueString) {
    for (char const & character : valueString) {
        if (std::isdigit(character) == 0) 
            return false;
    }
    return true;
}

char
Parser::getEscapedCharacter(const std::string & escapeSequence) {
	if (escapeSequence.at(0) == '\\') {
		switch (escapeSequence.at(1)) {
			case '?':
				return '?';
				break;
			case '\\':
				return '\\';
				break;
			case 'b':
				return '\b';
				break;
			case 'n':
				return '\n';
				break;
			case 'r':
				return '\r';
				break;
			case 't':
				return '\t';
				break;
			case 's':
				return ' ';
				break;
		}

        Format::printError(std::string("Bad escape sequence: ") + escapeSequence);
		return 0;
	}

	return escapeSequence.at(0);	
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