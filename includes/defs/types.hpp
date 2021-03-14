#pragma once

#include <string>
#include <vector>
#include <memory>

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

    using TypePtr = std::shared_ptr<Type>;

    class IntType : public Type {
        public:
            IntType() : Type(DataTypes::INT) { }
    };

    using IntTypePtr = std::shared_ptr<IntType>;

    class CharType : public Type {
        public:
            CharType() : Type(DataTypes::CHAR) { }
    };

    using CharTypePtr = std::shared_ptr<CharType>;

    class BoolType : public Type {
        public:
            BoolType() : Type(DataTypes::BOOL) { }
    };

    using BoolTypePtr = std::shared_ptr<BoolType>;

    class NullType : public Type {
        public:
            NullType() : Type(DataTypes::NULLVAL) { }
    };

    using NullTypePtr = std::shared_ptr<NullType>;

    class ListType : public Type {
        public:
            TypePtr listType;

            ListType(const TypePtr & listType)
            : Type(DataTypes::LIST),
              listType(listType) { }
    };

    using ListTypePtr = std::shared_ptr<ListType>;

    class GenType : public Type {
        public:
            const std::string identifier;
            
            GenType(const std::string & identifier)
            : Type(DataTypes::GEN),
              identifier(identifier) { }
    };

    using GenTypePtr = std::shared_ptr<GenType>;
    
    class FuncType : public Type {
        public:
            std::vector<GenTypePtr> genericTypes;
            std::vector<TypePtr> argumentTypes;
            TypePtr returnType;

            FuncType(const std::vector<GenTypePtr> & genericTypes,
                     const std::vector<TypePtr> & argumentTypes,
                     const TypePtr & returnType)
            : Type(DataTypes::FUNC),
              genericTypes(genericTypes),
              argumentTypes(argumentTypes),
              returnType(returnType) { }
    };

    using FuncTypePtr = std::shared_ptr<FuncType>;
}