#pragma once

#include "../../utils/logger.hpp"
#include "../../utils/prettyprint.hpp"
#include "../../defs/expressions.hpp"

#include <memory>

using namespace Expressions;

class CPSConverter {
    private:
        ExpPtr rootExpression;
        
        ExpPtr convert(ExpPtr expression);

        ExpPtr convertProgram(ExpPtr expression);
        ExpPtr convertLiteral(ExpPtr expression);
        ExpPtr convertPrimitive(ExpPtr expression);
        ExpPtr convertLet(ExpPtr expression);
        ExpPtr convertListDefinition(ExpPtr expression);
        ExpPtr convertTupleDefinition(ExpPtr expression);

        std::shared_ptr<Let> makeNewLet(Types::TypePtr valueType, ExpPtr value);
        std::shared_ptr<Reference> makeReference(ExpPtr value);
        std::shared_ptr<Let> getNestedLet(std::shared_ptr<Let> let);

        std::string newLetIdent();

    public:
        explicit CPSConverter(const ExpPtr & rootExpression);
        void convert();
};