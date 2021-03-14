#include "../includes/parser.hpp"

Parser::Parser(const std::vector<Token> & tokenStream)
: tokenStream(tokenStream) { }

ExpPtr
Parser::makeTree() {
    ExpPtr tree = parseProgram();
    // err if anything leftover
    return tree;
}

ExpPtr
Parser::parseProgram() {
    std::vector<std::shared_ptr<Function>> functions;
    while (match(Token::TokenType::KEYWORD, "func"))
        functions.push_back(parseFunc());

    return std::make_shared<Program>(currentToken(), functions, parseExpression());
}

ExpPtr
Parser::parseExpression() {
    if (!inBounds())
        return Expression::End();

    if (match(Token::TokenType::KEYWORD, "val")) {
        const std::string ident = currentToken().text;
        Token token = currentToken();
        advance();

        skip(":");
        Types::TypePtr valueType = parseType({});
        skip("=");
        ExpPtr valueExpression = parseSimpleExpression();
        skip(";");
        ExpPtr afterExpression = parseExpression();
        return std::make_shared<Let>(token, ident, valueType, valueExpression, afterExpression);
    } else {
        Token token = currentToken();
        ExpPtr simpleExpression = parseSimpleExpression();
        if (match(Token::TokenType::DELIM, ";")) {
            ExpPtr expression = parseExpression();
            return std::make_shared<Let>(token, dummy(), Types::NullTypePtr(), simpleExpression, expression);
        }
        return simpleExpression;
    }
}

ExpPtr
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

std::shared_ptr<Branch>
Parser::parseBranch() {
    const Token token = currentToken();
    skip("(");
    ExpPtr condition = parseSimpleExpression();
    skip(")");

    ExpPtr trueBranch = parseSimpleExpression();
    if (match(Token::TokenType::KEYWORD, "else")) {
        return std::make_shared<Branch>(token, condition, 
                                        trueBranch, parseSimpleExpression());
    }
    return std::make_shared<Branch>(token, condition, 
                                    trueBranch, std::make_shared<Literal>(token));
}

std::shared_ptr<ListDefinition>
Parser::parseList() {
    const Token token = currentToken();
    skip("List");
	skip("{");

    std::vector<ExpPtr> listValues;
    if (inBounds() && currentToken().text != "}") {
        listValues.push_back(parseSimpleExpression());
        while (match(Token::TokenType::DELIM, ",")) {
            listValues.push_back(parseSimpleExpression());
        }
    }

    skip("}");
    return std::make_shared<ListDefinition>(token, listValues);
}

std::shared_ptr<Match>
Parser::parseMatch() {
    const Token token = currentToken();
    skip("(");
    const std::string ident = currentToken().text;
    advance();
    skip(")");
	skip("{");

    std::vector<std::shared_ptr<Case>> cases;
    while (match(Token::TokenType::KEYWORD, "case")) {
        cases.push_back(parseCase());
    }
    skip("}");
    return std::make_shared<Match>(token, ident, cases);
}

// DONE
std::shared_ptr<Case>
Parser::parseCase() {
    const Token token = currentToken();
    ExpPtr ident; // making this a pointer avoids calling private constructor to init
    if (match(Token::TokenType::KEYWORD, "any")) {
        ident = std::make_shared<Reference>(token, Types::NullTypePtr(), std::string("$any"));
    } else {
        ident = parseAtom();
    }
    skip("=");
    skip("{");
    ExpPtr block = parseSimpleExpression();
    skip("}");
    skip(";");
    return std::make_shared<Case>(token, ident, block);
}

// DONE
ExpPtr
Parser::parseUtight(int min) {
    ExpPtr leftSide = parseUtight();

    while (inBounds() && Operator::isBinaryOperator(currentToken().text, min)) {
        Token token = currentToken();
        Operator::OperatorTypes op = Operator::getOperator(currentToken().text);
        advance();

        int tempMin = Operator::getPrecedence(op) + 1;
        ExpPtr rightSide = parseUtight(tempMin);
        leftSide = std::make_shared<Primitive>(token, leftSide->returnType, op, leftSide, rightSide);
    }
    return leftSide;
}

ExpPtr
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

    ExpPtr rightSide = parseTight();
    Token token = currentToken();
    if (op == Operator::OperatorTypes::PLUS || op == Operator::OperatorTypes::MINUS) {
		return std::make_shared<Primitive>(token, Types::IntTypePtr(), op, std::make_shared<Literal>(token, Types::IntTypePtr(), 0), rightSide);
	} else if (op == Operator::OperatorTypes::NOT) {
		return std::make_shared<Primitive>(token, Types::BoolTypePtr(), op, std::make_shared<Literal>(token, Types::BoolTypePtr(), false), rightSide);
	}
	return rightSide;
}

ExpPtr
Parser::parseTight() {
    if (match(Token::TokenType::DELIM, "{")) {
        ExpPtr exp = parseExpression();
        skip("}");
        return exp;
    } else if (peek().type == Token::TokenType::DELIM && peek().text == "[") {
        return parseBlockGet();
    }
    return parseApplication();
}

std::shared_ptr<BlockGet>
Parser::parseBlockGet() {
    Token token = currentToken();

    ExpPtr reference = parseAtom();
    skip("[");
	ExpPtr index = parseSimpleExpression();
	skip("]");
    std::shared_ptr<BlockGet> blockGet = std::make_shared<BlockGet>(token, reference, index);
    while (match(Token::TokenType::DELIM, "[")) {
        std::shared_ptr<BlockGet> blockGetInner = std::make_shared<BlockGet>(currentToken(), blockGet->reference, blockGet->index);
        blockGet->reference = blockGetInner;
        blockGet->index = parseSimpleExpression();
        skip("]");
    }

	return blockGet;
}

std::shared_ptr<Application>
Parser::parseApplication() {
    Token token = currentToken();

    ExpPtr ident = parseAtom();
    if (match(Token::TokenType::DELIM, "(")) {
        std::vector<ExpPtr> arguments;
        skip("(");
        if (inBounds() && currentToken().text != ")") {
            arguments.push_back(parseSimpleExpression());
        }
        while (match(Token::TokenType::DELIM, ",")) {
            arguments.push_back(parseSimpleExpression());	
        }
        skip(")");

        std::shared_ptr<Application> app = std::make_shared<Application>(token, ident, arguments);

        while (match(Token::TokenType::DELIM, "(")) {
                token = currentToken();

				std::vector<ExpPtr> argumentsInner;
                skip("(");
                if (inBounds() && currentToken().text != ")") {
                    argumentsInner.push_back(parseSimpleExpression());
                }
                while (match(Token::TokenType::DELIM, ",")) {
                    argumentsInner.push_back(parseSimpleExpression());	
                }
				skip(")");
                std::shared_ptr<Application> appInner = std::make_shared<Application>(token, ident, arguments);
                app->functionIdent = appInner;
                app->arguments = argumentsInner;
			}
			return app;
    }
    return std::static_pointer_cast<Application>(ident);
}

std::shared_ptr<Function>
Parser::parseFunc() {
    return std::static_pointer_cast<Function>(Expression::End());
}

std::shared_ptr<Argument>
Parser::parseArg(const std::vector<Types::GenTypePtr> & genericParameterList) {
    const std::string argumentName = currentToken().text;
    Token token = currentToken();
    advance();
    skip(":");
    return std::make_shared<Argument>(token, parseType(genericParameterList), argumentName);
}

ExpPtr
Parser::parseAtom() {
    if (match(Token::TokenType::DELIM, "(")) {
        ExpPtr exp = parseSimpleExpression();
        skip(")");
        return exp;
    } else if (inBounds()) {
        if (currentToken().type == Token::TokenType::IDENT) {

        } else {
            Token token = currentToken();
            if (matchNoAdvance(Token::TokenType::KEYWORD, "true") ||
                matchNoAdvance(Token::TokenType::KEYWORD, "false")) {
                std::shared_ptr<Literal> lit = std::make_shared<Literal>(currentToken(), Types::BoolTypePtr(), (currentToken().text == "true"));
                advance();
                return lit;
            } else if (match(Token::TokenType::KEYWORD, "null")) {
                return std::make_shared<Literal>(token);
            } else if (isValue(currentToken().text)) {
                std::shared_ptr<Literal> lit = std::make_shared<Literal>(currentToken(), Types::BoolTypePtr(), std::stoi(currentToken().text));
                advance();
                return lit;
            } else if (match(Token::TokenType::DELIM, "'") && currentToken().text.length() <= 2) {
                std::shared_ptr<Literal> lit = std::make_shared<Literal>(currentToken(), Types::CharTypePtr(), getEscapedCharacter(currentToken().text));
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

Types::TypePtr
Parser::parseType(const std::vector<Types::GenTypePtr> & genericParameterList) {
    if (inBounds() && currentToken().type == Token::TokenType::KEYWORD && currentToken().text != "List") {
		const std::string typeString = currentToken().text;
        
		Types::TypePtr type;
		if (typeString == "int") type = Types::IntTypePtr();
		else if (typeString == "bool") type = Types::BoolTypePtr();
		else if (typeString == "char") type = Types::CharTypePtr();
		else if (typeString == "null") type = Types::NullTypePtr();
		else {
			Format::printError(std::string("Unexpected type: ") + typeString);
			return Types::NullTypePtr();
		}
		advance();
		
		if (match(Token::TokenType::DELIM, "->")) {
            std::vector<Types::TypePtr> functionTypeArgumentTypes{type};
            return std::make_shared<Types::FuncType>(genericParameterList, functionTypeArgumentTypes, parseType(genericParameterList));
		}

		return type;
	} else if (match(Token::TokenType::KEYWORD, "List")) {
        skip("[");
        Types::TypePtr listDataType = parseType(genericParameterList);
        skip("]");
        return std::make_shared<Types::ListType>(listDataType);
    } else if (match(Token::TokenType::DELIM, "(")) {
        std::vector<Types::TypePtr> functionArgumentTypes{parseType(genericParameterList)};
        while (match(Token::TokenType::DELIM, ",")) {
			functionArgumentTypes.push_back(parseType(genericParameterList));	
		}
		skip(")");
		skip("->");
		return std::make_shared<Types::FuncType>(genericParameterList, functionArgumentTypes, parseType(genericParameterList));
    } else {
        const std::string parameterName = currentToken().text;
		bool genericNameMatches = false;

		Types::TypePtr type;
		for (const auto genericParameterType : genericParameterList) {
			if (genericParameterType->identifier == parameterName) {
				type = std::make_shared<Types::GenType>(parameterName);
				genericNameMatches = true;
				advance();
				break;
			}
		}

		if (!genericNameMatches) {
            Format::printError(std::string("Undefined generic type: ") + parameterName);
			return Types::NullTypePtr();
		}

		return type;
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