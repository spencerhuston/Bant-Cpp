#pragma once

#include <algorithm>

namespace Operator {
    enum class OperatorTypes {
        PLUS, MINUS, TIMES, DIV, MOD, // arithmetic
        GRT, LST, NOT, EQ, NOTEQ, GRTEQ, LSTEQ, AND, OR, // comparison
        NONE 
    };

    inline OperatorTypes
    getOperator(const std::string & text) {
        OperatorTypes op;
        if (text == "+") op = OperatorTypes::PLUS;
        else if (text == "-") op = OperatorTypes::MINUS;
        else if (text == "*") op = OperatorTypes::TIMES;
        else if (text == "/") op = OperatorTypes::DIV;
        else if (text == "%") op = OperatorTypes::MOD;
        else if (text == ">") op = OperatorTypes::GRT;
        else if (text == "<") op = OperatorTypes::LST;
        else if (text == "==") op = OperatorTypes::EQ;
        else if (text == "!=") op = OperatorTypes::NOTEQ;
        else if (text == ">=") op = OperatorTypes::GRTEQ;
        else if (text == "<=") op = OperatorTypes::LSTEQ;
        else if (text == "&&") op = OperatorTypes::AND;
        else if (text == "||") op = OperatorTypes::OR;
        return op;
    }

    inline int
    getPrecedence(const OperatorTypes op) {
        if (op == OperatorTypes::AND || op == OperatorTypes::OR) return 0;
        else if (op == OperatorTypes::PLUS || op == OperatorTypes::MINUS) return 2;
        else if (op == OperatorTypes::TIMES || op == OperatorTypes::DIV || op == OperatorTypes::MOD) return 3;
        return 1;
    }

    inline bool
    isUnaryOperator(const std::string & op) {
        return (op == std::string("+") || op == std::string("-") || op == std::string("!"));
    }

    inline bool
    isBinaryOperator(const std::string & op, int min) {
        static std::array<std::string, 14> ops = {
            "+", "-", "*", "/", "%", "&&", "||",
            "<", ">", "!", "==", "!=", "<=", ">="
        };

        return ((std::find(ops.begin(), ops.end(), op) != ops.end()) && 
                (getPrecedence(getOperator(op)) >= min));
    }

    inline bool 
    isUnaryOperator(const OperatorTypes & op) {
        if (op == OperatorTypes::PLUS ||
            op == OperatorTypes::MINUS || 
            op == OperatorTypes::NOT) {
            return true;
        }
        return false;
    }

    inline bool
    isComparisonOperator(const OperatorTypes & op) {
        if (op == OperatorTypes::EQ ||
            op == OperatorTypes::NOTEQ || 
            op == OperatorTypes::GRTEQ || 
            op == OperatorTypes::LSTEQ ||
            op == OperatorTypes::AND || 
            op == OperatorTypes::OR) {
            return true;
        }
        return false;
    }

    inline bool
    isArithmeticOperator(const OperatorTypes & op) {
        if (op == OperatorTypes::PLUS ||
            op == OperatorTypes::MINUS || 
            op == OperatorTypes::TIMES || 
            op == OperatorTypes::DIV ||
            op == OperatorTypes::MOD) {
            return true;
        }
        return false;
    }
}