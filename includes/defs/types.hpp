#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Types {
    enum class DataTypes {
        INT,
        CHAR,
        STRING,
        BOOL,
        NULLVAL,
        LIST,
        TUPLE,
        FUNC,
        GEN,
        TYPECLASS
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
                    case DataTypes::STRING:
                        return "STRING";
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
                    case DataTypes::TUPLE:
                        return "TUPLE";
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

    class StringType : public Type {
        public:
            StringType() : Type(DataTypes::STRING) { }
    };

    using StringTypePtr = std::shared_ptr<StringType>;

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

    class TupleType : public Type {
        public:
            std::vector<TypePtr> tupleTypes;

            TupleType(const std::vector<TypePtr> & tupleTypes)
            : Type(DataTypes::TUPLE),
              tupleTypes(tupleTypes) { }
    };

    using TupleTypePtr = std::shared_ptr<TupleType>;

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

    class TypeclassType : public Type {
        public:
            const std::string ident;
            std::vector<TypePtr> fieldTypes;

            TypeclassType(const std::string & ident,
                          const std::vector<TypePtr> & fieldTypes)
            : Type(DataTypes::TYPECLASS),
              ident(ident),
              fieldTypes(fieldTypes) { }

            TypeclassType(const std::string & ident)
            : Type(DataTypes::TYPECLASS),
              ident(ident),
              fieldTypes({}) { }
    };

    using TypeclassTypePtr = std::shared_ptr<TypeclassType>;
}