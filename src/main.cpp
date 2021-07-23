#include <string>
#include <iostream>
#include <sstream>

#include "core/lexer/lexer.hpp"
#include "core/parser/parser.hpp"
#include "core/typeChecker/typeChecker.hpp"
#include "core/interpreter/interpreter.hpp"
#include "core/builtin/builtinImplementations.hpp"

int main(int argc, char ** argv) {
    std::string sourceStream;

    if (argc == 1) {
        Format::printError("Error: Source file required");
        exit(1);
    } else if (argc == 2) {
        sourceStream = Lexer::readFile(std::string(argv[1]));
    } else if (argc == 3) {
        if (std::string(argv[1]) == "-d") {
            Format::debug = true;
            sourceStream = Lexer::readFile(std::string(argv[2]));
        } else {
            sourceStream = Lexer::readFile(std::string(argv[1]));
            if (std::string(argv[1]) == "-d") {
                Format::debug = true;
            } else {
                Format::printError(std::string("Unknown argument: ") + std::string(argv[1]));
                exit(1);
            }
        }
    }

    if (sourceStream.empty())
        exit(2);
    
    int phase = 0;

    //try {
        Format::printDebugHeader("Building...");

        auto lexer = Lexer(BuiltinDefinitions::builtinDefinitions + sourceStream);
        auto tokenStream = lexer.makeTokenStream();

        if (lexer.errorOccurred()) {
            Format::printError("One or more errors occurred during lexing, exiting");
            exit(3);
        }

        phase++;

        auto parser = Parser(tokenStream);
        auto tree = parser.makeTree();

        if (parser.errorOccurred()) {
            Format::printError("One or more errors occurred during parsing, exiting");
            exit(4);
        }

        phase++;

        auto typeChecker = TypeChecker(tree);
        typeChecker.check();

        if (typeChecker.errorOccurred()) {
            Format::printError("One or more errors occurred during type checking, exiting");
            exit(5);
        }

        Format::printDebugHeader("Successful Build, Running...");

        phase++;
        
        auto interpreter = Interpreter(tree);
        BuiltinImplementations::interpreter = interpreter;
        interpreter.run();

        if (interpreter.errorOccurred()) {
            Format::printError("One or more errors occurred at runtime, exiting");
            exit(6);
        }
    /*} catch (...) {
        std::string errorExitMessage = "Unexpected error occurred";
        switch (phase) {
            case 0:
                errorExitMessage += std::string(" during lexing");
                break;
            case 1:
                errorExitMessage += std::string(" during parsing");
                break;
            case 2:
                errorExitMessage += std::string(" during type checking");
                break;
            case 3:
                errorExitMessage += std::string(" during interpretation");
                break;
            default:
                break;
        }
        errorExitMessage += std::string(", exiting");

        Format::printError(errorExitMessage);
        exit(7);
    }*/
}