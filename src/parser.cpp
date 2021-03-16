#include "../includes/parser.hpp"

Parser::Parser(const std::vector<Token> & tokenStream)
: tokenStream(tokenStream) { }

ExpPtr
Parser::makeTree() {
    Format::printDebugHeader("Parsing");
    preprocessImports();
    ExpPtr tree = parseProgram();
    // err if anything leftover
    Format::printDebugHeader("Parsing Done");
    return tree;
}

std::shared_ptr<Program>
Parser::parseProgram() {
    Token token = currentToken();

    std::vector<std::shared_ptr<Function>> functions;
    while (match(Token::TokenType::KEYWORD, "func"))
        functions.push_back(parseFunc());

    return std::make_shared<Program>(token, functions, parseExpression());
}

void
Parser::preprocessImports() {
    bool importing = false;
    do {
        importing = false;
        for (unsigned int tokenIndex = 0; tokenIndex < tokenStream.size(); ++tokenIndex) {
            if (tokenStream.at(tokenIndex).text == "import") {
                importing = true;
                auto newStream = parseImport(tokenIndex);
                
                if (!newStream.empty())
                    tokenStream.insert(tokenStream.begin() + tokenIndex, newStream.begin(), newStream.end());
            }
        }
    } while (importing);
}

std::vector<Token>
Parser::parseImport(int tokenIndex) {
    tokenStream.erase(tokenStream.begin() + tokenIndex); // remove import token

    std::string sourceFileName = tokenStream.at(tokenIndex).text;
    tokenStream.erase(tokenStream.begin() + tokenIndex); // remove file name token, now on first token of next

    while (tokenStream.at(tokenIndex).text == "/") {
        sourceFileName += tokenStream.at(tokenIndex).text; // add "/"
        tokenStream.erase(tokenStream.begin() + tokenIndex);

        sourceFileName += tokenStream.at(tokenIndex).text; // nested file
        tokenStream.erase(tokenStream.begin() + tokenIndex);
    }

    std::string stream = Lexer::readFile(sourceFileName + std::string(".bnt"));

    if (stream.empty())
        return std::vector<Token>{};

    auto lexer = Lexer(std::move(stream));
    return lexer.makeTokenStream();
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
            return std::make_shared<Let>(token, dummy(), std::make_shared<Types::NullType>(), simpleExpression, expression);
        }
        return simpleExpression;
    }
}

ExpPtr
Parser::parseSimpleExpression() {
    if (match(Token::TokenType::KEYWORD, "if")) {
        return parseBranch();
    } else if (match(Token::TokenType::KEYWORD, "List")) {
        return parseList();
    } else if (match(Token::TokenType::KEYWORD, "Tuple")) {
        return parseTuple();
    } else if (match(Token::TokenType::KEYWORD, "match")) {
        return parseMatch();
    } else if (match(Token::TokenType::KEYWORD, "typeclass")) {
        return parseTypeclass();
    } else if (matchNoAdvance(Token::TokenType::KEYWORD, "func")) {
        return parseProgram();
    }
    return parseUtight(0);
}

std::shared_ptr<Typeclass>
Parser::parseTypeclass() {
    Token token = currentToken();
    const std::string ident = currentToken().text;
    advance();
    skip("{");

    std::vector<std::shared_ptr<Argument>> fields;
    if (inBounds() && currentToken().type != Token::TokenType::DELIM && currentToken().text != "}") {
        fields.push_back(parseArg({}));
        while (match(Token::TokenType::DELIM, ",")) {
            fields.push_back(parseArg({}));
        }
    }
    skip("}");

    std::vector<Types::TypePtr> fieldTypes;
    for (const auto & field : fields) {
        fieldTypes.push_back(field->returnType);
    }

    Types::TypeclassTypePtr type = std::make_shared<Types::TypeclassType>(ident, fieldTypes);
    return std::make_shared<Typeclass>(token, ident, fields, type);
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

std::shared_ptr<TupleDefinition>
Parser::parseTuple() {
    const Token token = currentToken();
	skip("{");

    std::vector<ExpPtr> tupleValues;
    if (inBounds() && currentToken().text != "}") {
        tupleValues.push_back(parseSimpleExpression());
        while (match(Token::TokenType::DELIM, ",")) {
            tupleValues.push_back(parseSimpleExpression());
        }
    }

    skip("}");
    return std::make_shared<TupleDefinition>(token, tupleValues);
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

std::shared_ptr<Case>
Parser::parseCase() {
    const Token token = currentToken();
    ExpPtr ident; // making this a pointer avoids calling private constructor to init
    if (match(Token::TokenType::KEYWORD, "any")) {
        ident = std::make_shared<Reference>(token, std::make_shared<Types::NullType>(), std::string("$any"));
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
    Token token = currentToken();
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
    if (op == Operator::OperatorTypes::PLUS || op == Operator::OperatorTypes::MINUS) {
		return std::make_shared<Primitive>(token, std::make_shared<Types::IntType>(), op, std::make_shared<Literal>(token, std::make_shared<Types::IntType>(), 0), rightSide);
	} else if (op == Operator::OperatorTypes::NOT) {
		return std::make_shared<Primitive>(token, std::make_shared<Types::BoolType>(), op, std::make_shared<Literal>(token, std::make_shared<Types::BoolType>(), false), rightSide);
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
    const std::string functionName = currentToken().text;
    Token token = currentToken();
    advance();

    std::vector<Types::GenTypePtr> genericTypes;
    if (match(Token::TokenType::DELIM, "[")) {
        Types::GenTypePtr genericType = std::make_shared<Types::GenType>(currentToken().text);
        advance();
        genericTypes.push_back(genericType);
        while (match(Token::TokenType::DELIM, ",")) {
            Types::GenTypePtr genericType2 = std::make_shared<Types::GenType>(currentToken().text);
            advance();
            genericTypes.push_back(genericType2);
        }
        skip("]");
    }

    skip("(");
    std::vector<std::shared_ptr<Argument>> argumentTypes;
    if (inBounds() && currentToken().type != Token::TokenType::DELIM && currentToken().text != ")") {
        argumentTypes.push_back(parseArg(genericTypes));
        while (match(Token::TokenType::DELIM, ",")) {
            argumentTypes.push_back(parseArg(genericTypes));
        }
    }
    skip(")");
	
    skip("->");
    Types::TypePtr functionReturnType = parseType(genericTypes);
	skip("=");

	ExpPtr functionBody = parseSimpleExpression();
    std::vector<Types::TypePtr> functionTypeArgumentTypes;
    for (const auto & argumentType : argumentTypes) {
        functionTypeArgumentTypes.push_back(argumentType->returnType);
    }

    Types::FuncTypePtr functionType = std::make_shared<Types::FuncType>(genericTypes, functionTypeArgumentTypes, functionReturnType);
	
    skip(";");
    return std::make_shared<Function>(token, functionType, functionName, genericTypes, argumentTypes, functionBody);
}

std::shared_ptr<Argument>
Parser::parseArg(const std::vector<Types::GenTypePtr> & genericParameterList) {
    const std::string argumentName = currentToken().text;
    Token token = currentToken();
    advance();
    skip(":");
    Types::TypePtr argumentType = parseType(genericParameterList);

    ExpPtr defaultValue;
    if (match(Token::TokenType::DELIM, "=")) {
        defaultValue = parseAtom();
    } else {
        defaultValue = nullptr;
    }

    return std::make_shared<Argument>(token, argumentType, argumentName, defaultValue);
}

ExpPtr
Parser::parseAtom() {
    if (match(Token::TokenType::DELIM, "(")) {
        ExpPtr exp = parseSimpleExpression();
        skip(")");
        return exp;
    } else if (inBounds()) {
        if (currentToken().type == Token::TokenType::IDENT) {
            Token token = currentToken();
            const std::string ident = currentToken().text;
		    advance();

            if (match(Token::TokenType::DELIM, ".")) {
                const std::string fieldIdent = currentToken().text;
                advance();
                return std::make_shared<Reference>(token, std::make_shared<Types::UnknownType>(), ident, fieldIdent);
            }
            
            return std::make_shared<Reference>(token, std::make_shared<Types::UnknownType>(), ident);
        } else {
            Token token = currentToken();
            if (matchNoAdvance(Token::TokenType::KEYWORD, "true") ||
                matchNoAdvance(Token::TokenType::KEYWORD, "false")) {
                std::shared_ptr<Literal> lit = std::make_shared<Literal>(currentToken(), std::make_shared<Types::BoolType>(), (currentToken().text == "true"));
                advance();
                return lit;
            } else if (match(Token::TokenType::KEYWORD, "null")) {
                return std::make_shared<Literal>(token);
            } else if (isValue(currentToken().text)) {
                std::shared_ptr<Literal> lit = std::make_shared<Literal>(currentToken(), std::make_shared<Types::IntType>(), std::stoi(currentToken().text));
                advance();
                return lit;
            } else if (match(Token::TokenType::DELIM, "'") && currentToken().text.length() <= 2) {
                std::shared_ptr<Literal> lit = std::make_shared<Literal>(currentToken(), std::make_shared<Types::CharType>(), getEscapedCharacter(currentToken().text));
                advance();
                skip("'");
                return lit;
            } else if (match(Token::TokenType::DELIM, "\"")) {
                std::shared_ptr<Literal> lit = std::make_shared<Literal>(currentToken(), std::make_shared<Types::StringType>(), currentToken().text);
                advance();
                skip("\"");
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
    if (inBounds() && currentToken().type == Token::TokenType::KEYWORD && currentToken().text != "List" && currentToken().text != "Tuple") {
		std::string typeString = currentToken().text;
        
		Types::TypePtr type;
		if (typeString == "int") type = std::make_shared<Types::IntType>();
		else if (typeString == "bool") type = std::make_shared<Types::BoolType>();
		else if (typeString == "char") type = std::make_shared<Types::CharType>();
        else if (typeString == "string") type = std::make_shared<Types::StringType>();
		else if (typeString == "null") type = std::make_shared<Types::NullType>();
        else if (typeString == "type") {
            advance();
            typeString = currentToken().text;
            type = std::make_shared<Types::TypeclassType>(typeString);
        } else {
			Format::printError(std::string("Unexpected type: ") + typeString);
			return std::make_shared<Types::UnknownType>();
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
    } else if (match(Token::TokenType::KEYWORD, "Tuple")) {
        skip("[");
        std::vector<Types::TypePtr> tupleTypes{parseType(genericParameterList)};
        while (match(Token::TokenType::DELIM, ",")) {
			tupleTypes.push_back(parseType(genericParameterList));	
		}
		skip("]");
        return std::make_shared<Types::TupleType>(tupleTypes);
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
			return std::make_shared<Types::UnknownType>();
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
        printError(text, currentToken().text);
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
Parser::printError(const std::string & errorString, const std::string expected) {
    std::string expectedString = (expected != std::string("$")) ? (std::string(", Expected: ") + expected) : std::string(" ");
    error = true;
    FilePosition position = currentToken().position;
    std::stringstream errorStream;
    errorStream << "Line: " << position.fileLine
                << ", Column: " << position.fileColumn - 1 << std::endl
                << "Unexpected character: " << errorString << expectedString
                << std::endl << std::endl
                << position.currentLineText << std::endl
                << std::string(position.fileColumn - errorString.length() - 1, ' ') << "^";
    Format::printError(errorStream.str());
}