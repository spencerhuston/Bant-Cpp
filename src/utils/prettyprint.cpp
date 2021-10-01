#include "prettyprint.hpp"

PrettyPrint::PrettyPrint() {
    colors = {
        "\033[1;32m", // GREEN
        "\033[1;34m", // BLUE
        "\033[1;33m", // YELLOW
        "\033[1;35m", // MAGENTA
        "\033[1;31m", // RED
        "\033[1;36m", // CYAN
        "\033[1;37m", // WHITE
        "\033[1;92m", // BRIGHT_GREEN
        "\033[1;94m", // BRIGHT_BLUE
        "\033[1;93m", // BRIGHT_YELLOW
        "\033[1;95m", // BRIGHT_MAGENTA
        "\033[1;91m", // BRIGHT_RED
        "\033[1;96m", // BRIGHT_CYAN
        "\033[1;97m", // BRIGHT_WHITE
    };
}

void PrettyPrint::print(ExpPtr expression) {
    if (expression->expType == ExpressionTypes::PROG)
        printProgram(expression);
    else if (expression->expType == ExpressionTypes::LIT)
        printLiteral(expression);
    else if (expression->expType == ExpressionTypes::PRIM)
        printPrimitive(expression);
    else if (expression->expType == ExpressionTypes::LET)
        printLet(expression);
    else if (expression->expType == ExpressionTypes::REF)
        printReference(expression);
    else if (expression->expType == ExpressionTypes::BRANCH)
        printBranch(expression);
    else if (expression->expType == ExpressionTypes::TYPECLASS)
        printTypeclass(expression);
    else if (expression->expType == ExpressionTypes::APP)
        printApplication(expression);
    else if (expression->expType == ExpressionTypes::LIST_DEF)
        printListDefinition(expression);
    else if (expression->expType == ExpressionTypes::TUPLE_DEF)
        printTupleDefinition(expression);
    else if (expression->expType == ExpressionTypes::MATCH)
        printMatch(expression);
}

void PrettyPrint::printProgram(ExpPtr expression) {
    auto program = std::static_pointer_cast<Program>(expression);
    printLine("PROGRAM");
    
    spaceCount++;

    print("functions:");
    if (program->functions.empty())
        printLineEnd("NONE");
    else {
        spaceCount++;
        printLineEnd("");
        for (const auto & function : program->functions)
            printFunctionDefinition(function);
        spaceCount--;
    }

    print(program->body);
    spaceCount--;
}

void PrettyPrint::printFunctionDefinition(ExpPtr expression) {
    auto function = std::static_pointer_cast<Function>(expression);
    
    printLine("FUNCTION");
    spaceCount++;

    printLine("name: " + function->name);
    printLine("return type: " + function->returnType->toString());

    print("generic parameters: ");
    if (function->genericParameters.empty())
        printLineEnd("NONE");
    else {
        spaceCount++;
        printLineEnd("");
        for (const auto & genericParameter : function->genericParameters) {
            printLine(genericParameter->toString());
        }
        spaceCount--;
    }

    print("parameters: ");
    if (function->parameters.empty())
        printLineEnd("NONE");
    else {
        spaceCount++;
        printLineEnd("");
        for (const auto & parameter : function->parameters) {
            printLine(parameter->name + " : " + parameter->returnType->toString());
        }
        spaceCount--;
    }

    printLine("body:");
    spaceCount++;
    print(function->functionBody);
    spaceCount--;

    spaceCount--;
}

void PrettyPrint::printLiteral(ExpPtr expression) {
    auto literal = std::static_pointer_cast<Literal>(expression);

    if (literal->returnType->dataType == Types::DataTypes::INT)
        print("INT LITERAL: " + std::to_string(literal->getData<int>()));
    else if (literal->returnType->dataType == Types::DataTypes::CHAR)
        print("CHAR LITERAL: " + std::string(1, literal->getData<char>()));
    else if (literal->returnType->dataType == Types::DataTypes::STRING)
        print("STRING LITERAL: " + literal->getData<std::string>());
    else if (literal->returnType->dataType == Types::DataTypes::BOOL)
        print("BOOL LITERAL: " + std::string(literal->getData<bool>() ? "true" : "false"));
    printLineEnd("");
}

void PrettyPrint::printPrimitive(ExpPtr expression) {
    auto primitive = std::static_pointer_cast<Primitive>(expression);

    printLine("PRIMITIVE");
    spaceCount++;
    
    printLine("left side:");
    spaceCount++;
    print(primitive->leftSide);
    spaceCount--;

    static constexpr auto opToString = [](const Operator::OperatorTypes & op) -> const std::string {
        switch (op) {
            case Operator::OperatorTypes::PLUS: return "PLUS +";
            case Operator::OperatorTypes::MINUS: return "MINUS -";
            case Operator::OperatorTypes::TIMES: return "TIMES *";
            case Operator::OperatorTypes::DIV: return "DIV /";
            case Operator::OperatorTypes::MOD: return "MOD %";
            case Operator::OperatorTypes::GRT: return "GRT >";
            case Operator::OperatorTypes::LST: return "LST <";
            case Operator::OperatorTypes::NOT: return "NOT !";
            case Operator::OperatorTypes::EQ: return "EQ ==";
            case Operator::OperatorTypes::NOTEQ: return "NOTEQ !=";
            case Operator::OperatorTypes::GRTEQ: return "GRTEQ >=";
            case Operator::OperatorTypes::LSTEQ: return "LSTEQ <=";
            case Operator::OperatorTypes::AND: return "AND &&";
            case Operator::OperatorTypes::OR: return "OR ||";
            default:
                return "NONE";
                break;
        }
        return "NONE";
    };

    printLine("operator: " + opToString(primitive->op));
    
    printLine("right side:");
    spaceCount++;
    print(primitive->rightSide);
    spaceCount--;

    spaceCount--;
}

void PrettyPrint::printLet(ExpPtr expression) {
    auto let = std::static_pointer_cast<Let>(expression);

    printLine("LET");
    spaceCount++;
    
    printLine("name: " + let->ident);
    printLine("type: " + let->valueType->toString());
    printLine("value:");
    spaceCount++;
    print(let->value);
    spaceCount--;

    printLine("after:");
    spaceCount++;
    print(let->afterLet);
    spaceCount--;

    spaceCount--;
}

void PrettyPrint::printReference(ExpPtr expression) {
    auto reference = std::static_pointer_cast<Reference>(expression);

    printLine("REFERENCE: " + reference->ident + ((reference->fieldIdent != "") ? "." + reference->fieldIdent : ""));
}

void PrettyPrint::printBranch(ExpPtr expression) {
    auto branch = std::static_pointer_cast<Branch>(expression);

    printLine("BRANCH");
    spaceCount++;

    printLine("condition:");
    spaceCount++;
    print(branch->condition);
    spaceCount--;
    printLine("if branch:");
    spaceCount++;
    print(branch->ifBranch);
    spaceCount--;
    printLine("else branch:");
    spaceCount++;
    print(branch->elseBranch);
    spaceCount--;

    spaceCount--;
}

void PrettyPrint::printTypeclass(ExpPtr expression) {
    auto typeclass = std::static_pointer_cast<Typeclass>(expression);

    printLine("TYPECLASS: " + typeclass->ident);
    
    spaceCount++;
    print("fields: ");
    if (typeclass->fields.empty())
        printLineEnd("NONE");
    else {
        spaceCount++;
        printLineEnd("");
        for (const auto & field : typeclass->fields) {
            printLine(field->name + " : " + field->returnType->toString());
        }
        spaceCount--;
    }
    spaceCount--;
}

void PrettyPrint::printApplication(ExpPtr expression) {
    auto application = std::static_pointer_cast<Application>(expression);

    printLine("APPLICATION");
    spaceCount++;

    printLine("ident:");
    spaceCount++;
    print(application->ident);
    spaceCount--;

    spaceCount++;
    print("generic replacement types: ");
    if (application->genericReplacementTypes.empty())
        printLineEnd("NONE");
    else {
        spaceCount++;
        printLineEnd("");
        for (const auto & genericReplacement : application->genericReplacementTypes) {
            printLine(genericReplacement->toString());
        }
        spaceCount--;
    }
    spaceCount--;

    spaceCount++;
    print("arguments: ");
    if (application->arguments.empty())
        printLineEnd("NONE");
    else {
        spaceCount++;
        printLineEnd("");
        for (const auto & argument : application->arguments) {
            print(argument);
        }
        spaceCount--;
    }
    spaceCount--;

    spaceCount--;
}

void PrettyPrint::printListDefinition(ExpPtr expression) {
    auto listDefinition = std::static_pointer_cast<ListDefinition>(expression);

    printLine("LIST DEFINITION");

    spaceCount++;
    print("values: ");
    if (listDefinition->values.empty())
        printLineEnd("NONE");
    else {
        spaceCount++;
        printLineEnd("");
        for (const auto & value : listDefinition->values) {
            print(value);
        }
        spaceCount--;
    }
    spaceCount--;
}

void PrettyPrint::printTupleDefinition(ExpPtr expression) {
    auto tupleDefinition = std::static_pointer_cast<TupleDefinition>(expression);

    printLine("TUPLE DEFINITION");

    spaceCount++;
    print("values: ");
    if (tupleDefinition->values.empty())
        printLineEnd("NONE");
    else {
        spaceCount++;
        printLineEnd("");
        for (const auto & value : tupleDefinition->values) {
            print(value);
        }
        spaceCount--;
    }
    spaceCount--;
}

void PrettyPrint::printMatch(ExpPtr expression) {
    auto match = std::static_pointer_cast<Match>(expression);

    printLine("MATCH: " + match->ident);

    spaceCount++;
    print("cases: ");
    if (match->cases.empty())
        printLineEnd("NONE");
    else {
        spaceCount++;
        printLineEnd("");
        for (const auto & casePtr : match->cases) {
            printCase(casePtr);
        }
        spaceCount--;
    }
    spaceCount--;
}

void PrettyPrint::printCase(ExpPtr expression) {
    auto casePtr = std::static_pointer_cast<Case>(expression);

    printLine("CASE");
    spaceCount++;

    printLine("value:");
    spaceCount++;
    print(casePtr->ident);
    spaceCount--;
    printLine("body:");
    spaceCount++;
    print(casePtr->body);
    spaceCount--;

    spaceCount--;
}