#include "cpsConverter.hpp"

CPSConverter::CPSConverter(const ExpPtr & rootExpression)
: rootExpression(rootExpression) { }

void
CPSConverter::convert() {
    HEADER("CPS Conversion");
    convert(rootExpression);
    HEADER("CPS Conversion Done");

    HEADER("CPS AST");
    if (Logger::getInstance().getLevel() == DEBUG) {
        PrettyPrint printer;
        printer.print(rootExpression);
    }
}

ExpPtr
CPSConverter::convert(ExpPtr expression) {
    if (expression->expType == ExpressionTypes::PROG)
        return convertProgram(expression);
    else if (expression->expType == ExpressionTypes::LIT)
        return convertLiteral(expression);
    else if (expression->expType == ExpressionTypes::PRIM)
        return convertPrimitive(expression);
    else if (expression->expType == ExpressionTypes::LET)
        return convertLet(expression);
    /*
    else if (expression->expType == ExpressionTypes::BRANCH)
        return convertBranch(expression);
    else if (expression->expType == ExpressionTypes::APP)
        return convertApplication(expression);
    */
    else if (expression->expType == ExpressionTypes::LIST_DEF)
        return convertListDefinition(expression);
    else if (expression->expType == ExpressionTypes::TUPLE_DEF)
        return convertTupleDefinition(expression);
    /*
    else if (expression->expType == ExpressionTypes::MATCH)
        return convertMatch(expression);
    */
    else // reference, typeclass
        return expression;
}

ExpPtr
CPSConverter::convertProgram(ExpPtr expression) {
    auto program = std::static_pointer_cast<Program>(expression);
    // TODO
    program->body = convert(program->body);
    return program;
}

ExpPtr
CPSConverter::convertLiteral(ExpPtr expression) {
    auto literal = std::static_pointer_cast<Literal>(expression);
    return makeNewLet(literal->returnType, literal);
}

ExpPtr
CPSConverter::convertPrimitive(ExpPtr expression) {
    auto primitive = std::static_pointer_cast<Primitive>(expression);

    auto leftSide = convert(primitive->leftSide);
    auto rightSide = convert(primitive->rightSide);

    primitive->leftSide = makeReference(leftSide);
    primitive->rightSide = makeReference(rightSide);
    auto primLet = makeNewLet(primitive->returnType, primitive);

    if (leftSide->expType == ExpressionTypes::LET) {
        auto tempLet = getNestedLet(std::static_pointer_cast<Let>(leftSide));

        if (rightSide->expType == ExpressionTypes::REF)
            tempLet->afterLet = primLet;
        else
            tempLet->afterLet = rightSide;
    }

    if (rightSide->expType == ExpressionTypes::LET) {
        auto tempLet = getNestedLet(std::static_pointer_cast<Let>(rightSide));
        tempLet->afterLet = primLet;
    }

    if (leftSide->expType == ExpressionTypes::LET)
        return leftSide;
    else if (rightSide->expType == ExpressionTypes::LET)
        return rightSide;
    else
        return primLet;
}

ExpPtr
CPSConverter::convertLet(ExpPtr expression) {
    auto let = std::static_pointer_cast<Let>(expression);

    if (let->value->expType != ExpressionTypes::REF && 
        let->value->expType != ExpressionTypes::LIT &&
        let->value->expType != ExpressionTypes::LET) {
        auto temp = convert(let->value);
        
        if (temp->expType == ExpressionTypes::LET) {
            getNestedLet(std::static_pointer_cast<Let>(temp))->afterLet = let;
        }
        let->afterLet = convert(let->afterLet);
        return temp;
    }

    let->afterLet = convert(let->afterLet);
    return let;
}

ExpPtr
CPSConverter::convertListDefinition(ExpPtr expression) {
    auto listDefinition = std::static_pointer_cast<ListDefinition>(expression);

    std::vector<std::shared_ptr<Let>> letValues;
    for (auto & value : listDefinition->values) {
        if (value->expType != ExpressionTypes::REF) {
            auto letValue = std::static_pointer_cast<Let>(convert(value));
            value = makeReference(letValue);
            letValues.push_back(letValue);
        }
    }

    for (unsigned int letIndex = 0; letIndex < letValues.size() - 1; ++letIndex) {
        getNestedLet(letValues.at(letIndex))->afterLet = letValues.at(letIndex + 1);
    }

    auto listLet = makeNewLet(listDefinition->returnType, listDefinition);

    getNestedLet(letValues.back())->afterLet = listLet;
    return letValues.front();
}

ExpPtr
CPSConverter::convertTupleDefinition(ExpPtr expression) {
    auto tupleDefinition = std::static_pointer_cast<TupleDefinition>(expression);

    std::vector<std::shared_ptr<Let>> letValues;
    for (auto & value : tupleDefinition->values) {
        if (value->expType != ExpressionTypes::REF) {
            auto letValue = std::static_pointer_cast<Let>(convert(value));
            value = makeReference(letValue);
            letValues.push_back(letValue);
        }
    }

    for (unsigned int letIndex = 0; letIndex < letValues.size() - 1; ++letIndex) {
        getNestedLet(letValues.at(letIndex))->afterLet = letValues.at(letIndex + 1);
    }

    auto tupleLet = makeNewLet(tupleDefinition->returnType, tupleDefinition);

    getNestedLet(letValues.back())->afterLet = tupleLet;
    return letValues.front();
}

std::shared_ptr<Let>
CPSConverter::makeNewLet(Types::TypePtr valueType, ExpPtr value) {
    auto ident = newLetIdent();
    return std::make_shared<Let>(value->token, ident, valueType, value, std::make_shared<Reference>(value->token, valueType, ident));
}

std::shared_ptr<Reference>
CPSConverter::makeReference(ExpPtr value) {
    if (value->expType == ExpressionTypes::REF)
        return std::static_pointer_cast<Reference>(value);
    
    auto let = getNestedLet(std::static_pointer_cast<Let>(value));
    return std::make_shared<Reference>(let->token, let->returnType, let->ident);
}

std::shared_ptr<Let>
CPSConverter::getNestedLet(std::shared_ptr<Let> let) {
    while (let->expType == ExpressionTypes::LET && let->afterLet->expType == ExpressionTypes::LET) {
        let = std::static_pointer_cast<Let>(let->afterLet);
    }
    return let;
}

std::string
CPSConverter::newLetIdent() {
    static int letCount = 0;
    return std::string("l$") + std::to_string(letCount++);
}