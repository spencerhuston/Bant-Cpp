#pragma once

#include <string>
#include <vector>

namespace Types {
    enum class DataTypes {
        INT,
        CHAR,
        BOOL,
        NULLVAL,
        LIST,
        FUNC,
        GEN
    };

    class Type {
        public:
            DataTypes dataType;

            Type(const DataTypes dataType)
            : dataType(dataType) { }

            const std::string
            getTypeAsString() {
                switch (dataType) {
                    case DataTypes::INT:
                        return "INT";
                        break;
                    case DataTypes::CHAR:
                        return "CHAR";
                        break;
                    case DataTypes::BOOL:
                        return "BOOL";
                        break;
                    case DataTypes::NULLVAL:
                        return "NULL";
                        break;
                    case DataTypes::LIST:
                        return "LIST";
                        break;
                    case DataTypes::FUNC:
                        return "FUNC";
                        break;
                    case DataTypes::GEN:
                        return "GEN";
                        break;
                    default:
                        return "UNKN";
                        break;
                }
                return "UNKN";
            }
    };

    class IntType : public Type {
        public:
            IntType() : Type(DataTypes::INT) { }
    };

    class CharType : public Type {
        public:
            CharType() : Type(DataTypes::CHAR) { }
    };

    class BoolType : public Type {
        public:
            BoolType() : Type(DataTypes::BOOL) { }
    };

    class NullType : public Type {
        public:
            NullType() : Type(DataTypes::NULLVAL) { }
    };

    class ListType : public Type {
        public:
            DataTypes listType;

            ListType(const DataTypes listType)
            : Type(DataTypes::LIST),
              listType(listType) { }
    };

    class GenType : public Type {
        public:
            std::string identifier;
            
            GenType(const std::string identifier)
            : Type(DataTypes::GEN),
              identifier(identifier) { }
    };
    
    class FuncType : public Type {
        public:
            std::vector<GenType> genericTypes;
            std::vector<Type> argumentTypes;
            DataTypes returnType;

            FuncType(const std::vector<GenType> genericTypes,
                     const std::vector<Type> argumentTypes,
                     const DataTypes returnType)
            : Type(DataTypes::FUNC),
              genericTypes(genericTypes),
              argumentTypes(argumentTypes),
              returnType(returnType) { }
    };
}