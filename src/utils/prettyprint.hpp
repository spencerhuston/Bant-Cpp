#pragma once

#include "../../defs/expressions.hpp"

using namespace Expressions;

class PrettyPrint {
    private:
        std::vector<std::string> colors;

        const std::string NO_COLOR = "\033[0m";
        int spaceCount = 0;

        void printTree(ExpPtr expression);
        void printProgram(ExpPtr expression);
        void printFunctionDefinition(ExpPtr expression);
        void printLiteral(ExpPtr expression);
        void printPrimitive(ExpPtr expression);
        void printLet(ExpPtr expression);
        void printReference(ExpPtr expression);
        void printBranch(ExpPtr expression);
        void printTypeclass(ExpPtr expression);
        void printApplication(ExpPtr expression);
        void printListDefinition(ExpPtr expression);
        void printTupleDefinition(ExpPtr expression);
        void printMatch(ExpPtr expression);
        void printCase(ExpPtr expression);

        void printLine(const std::string & str) const noexcept {
            std::cout << space() << colors.at(spaceCount % (colors.size() - 1)) << str << "\033[0m" << std::endl;
        }

        void print(const std::string & str) const noexcept {
            std::cout << space() << colors.at(spaceCount % (colors.size() - 1)) << str;
        }

        void printLineEnd(const std::string & str) const noexcept {
            std::cout << str << "\033[0m" << std::endl;
        }

        const std::string space() const noexcept {
            return std::string(spaceCount * 2, ' ');
        }

    public:
        explicit PrettyPrint();
        void print(ExpPtr expression);
};