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

        std::shared_ptr<Let> makeNewLet(Types::TypePtr valueType, ExpPtr value);
        std::string newLetIdent();

    public:
        explicit CPSConverter(const ExpPtr & rootExpression);
        void convert();
};