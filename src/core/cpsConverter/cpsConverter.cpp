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
    /*
    else if (expression->expType == ExpressionTypes::PRIM)
        return convertPrimitive(expression);
    else if (expression->expType == ExpressionTypes::LET)
        return convertLet(expression);
    else if (expression->expType == ExpressionTypes::REF)
        return expression;
    else if (expression->expType == ExpressionTypes::BRANCH)
        return convertBranch(expression);
    else if (expression->expType == ExpressionTypes::TYPECLASS)
        return expression;
    else if (expression->expType == ExpressionTypes::APP)
        return convertApplication(expression);
    else if (expression->expType == ExpressionTypes::LIST_DEF)
        return convertListDefinition(expression);
    else if (expression->expType == ExpressionTypes::TUPLE_DEF)
        return convertTupleDefinition(expression);
    else if (expression->expType == ExpressionTypes::MATCH)
        return convertMatch(expression);
    */
    else
        return expression;
}

ExpPtr
CPSConverter::convertProgram(ExpPtr expression) {
    auto program = std::static_pointer_cast<Program>(expression);
    program->body = convert(program->body);
    return program;
}

ExpPtr
CPSConverter::convertLiteral(ExpPtr expression) {
    auto literal = std::static_pointer_cast<Literal>(expression);
    return makeNewLet(literal->returnType, literal);
}

std::shared_ptr<Let>
CPSConverter::makeNewLet(Types::TypePtr valueType, ExpPtr value) {
    auto temp = std::make_shared<Temp>(value->token, valueType);
    return std::make_shared<Let>(value->token, newLetIdent(), valueType, value, temp);
}

std::string
CPSConverter::newLetIdent() {
    static int letCount = 0;
    return std::string("l$") + std::to_string(letCount++);
}