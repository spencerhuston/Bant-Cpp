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

            virtual const std::string toString() const = 0;

            bool compare(const std::shared_ptr<Type> & otherType) {
                auto dataTypeInt = static_cast<int>(dataType);
                auto otherTypeInt = static_cast<int>(otherType->dataType);

                if (otherTypeInt == static_cast<int>(DataTypes::GEN))
                    return true;

                return (dataTypeInt == otherTypeInt);
            }
    };

    using TypePtr = std::shared_ptr<Type>;

    class IntType : public Type {
        public:
            IntType() : Type(DataTypes::INT) { }

            const std::string toString() const override {
                return "int";
            }
    };

    using IntTypePtr = std::shared_ptr<IntType>;

    class CharType : public Type {
        public:
            CharType() : Type(DataTypes::CHAR) { }

            const std::string toString() const override {
                return "char";
            }
    };

    using CharTypePtr = std::shared_ptr<CharType>;

    class StringType : public Type {
        public:
            StringType() : Type(DataTypes::STRING) { }

            const std::string toString() const override {
                return "string";
            }
    };

    using StringTypePtr = std::shared_ptr<StringType>;

    class BoolType : public Type {
        public:
            BoolType() : Type(DataTypes::BOOL) { }

            const std::string toString() const override {
                return "bool";
            }
    };

    using BoolTypePtr = std::shared_ptr<BoolType>;

    class NullType : public Type {
        public:
            NullType() : Type(DataTypes::NULLVAL) { }

            const std::string toString() const override {
                return "null";
            }
    };

    using NullTypePtr = std::shared_ptr<NullType>;

    class ListType : public Type {
        public:
            TypePtr listType;

            ListType(const TypePtr & listType)
            : Type(DataTypes::LIST),
              listType(listType) { }

            const std::string toString() const override {
                return std::string("List[") + listType->toString() + std::string("]");
            }
    };

    using ListTypePtr = std::shared_ptr<ListType>;

    class TupleType : public Type {
        public:
            std::vector<TypePtr> tupleTypes;

            TupleType(const std::vector<TypePtr> & tupleTypes)
            : Type(DataTypes::TUPLE),
              tupleTypes(tupleTypes) { }

            const std::string toString() const override {
                std::string typeString("Tuple[");

                if (tupleTypes.empty()) {
                    typeString += std::string("]");
                } else if (tupleTypes.size() == 1) {
                    typeString += tupleTypes.at(0)->toString() + std::string("]");
                } else {
                    for (unsigned int typesIndex = 0; typesIndex < tupleTypes.size() - 1; ++typesIndex) {
                        typeString += tupleTypes.at(typesIndex)->toString() + std::string(", ");
                    }
                    typeString += tupleTypes.at(tupleTypes.size() - 1)->toString() + std::string("]");
                }

                return typeString;
            }
    };

    using TupleTypePtr = std::shared_ptr<TupleType>;

    class GenType : public Type {
        public:
            const std::string identifier;
            
            GenType(const std::string & identifier)
            : Type(DataTypes::GEN),
              identifier(identifier) { }
            
            const std::string toString() const override {
                return identifier;
            }
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
            
            const std::string toString() const override {
                std::string typeString("[");

                if (genericTypes.empty()) {
                    typeString += std::string("]");
                } else if (genericTypes.size() == 1) {
                    typeString += genericTypes.at(0)->toString() + std::string("]");
                } else {
                    for (unsigned int typesIndex = 0; typesIndex < genericTypes.size() - 1; ++typesIndex) {
                        typeString += genericTypes.at(typesIndex)->toString() + std::string(", ");
                    }
                    typeString += genericTypes.at(genericTypes.size() - 1)->toString() + std::string("]");
                }

                typeString += std::string("(");
                if (argumentTypes.empty()) {
                    typeString += std::string(")");
                } else if (argumentTypes.size() == 1) {
                    typeString += argumentTypes.at(0)->toString() + std::string(")");
                } else {
                    for (unsigned int typesIndex = 0; typesIndex < argumentTypes.size() - 1; ++typesIndex) {
                        typeString += argumentTypes.at(typesIndex)->toString() + std::string(", ");
                    }
                    typeString += argumentTypes.at(argumentTypes.size() - 1)->toString() + std::string(")");
                }
                
                typeString += std::string("->");
                typeString += returnType->toString();

                return typeString;
            }
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
            
            const std::string toString() const override {
                return ident;
            }
    };

    using TypeclassTypePtr = std::shared_ptr<TypeclassType>;
}