#include <string>
#include <iostream>
#include <sstream>

#include "core/lexer/lexer.hpp"
#include "core/parser/parser.hpp"
#include "core/typeChecker/typeChecker.hpp"
#include "core/interpreter/interpreter.hpp"
#include "core/builtin/builtinImplementations.hpp"

#include "utils/logger.hpp"

void runBant(const std::string & sourceStream) {
    int phase = 0;
    try {
        HEADER("Building...");

        auto lexer = Lexer(BuiltinDefinitions::builtinDefinitions + sourceStream);
        auto tokenStream = lexer.makeTokenStream();

        if (lexer.errorOccurred()) {
            ERROR("One or more errors occurred during lexing, exiting");
            return;
        }

        phase++;

        auto parser = Parser(tokenStream);
        auto tree = parser.makeTree();

        if (parser.errorOccurred()) {
            ERROR("One or more errors occurred during parsing, exiting");
            return;
        }

        phase++;

        auto typeChecker = TypeChecker(tree);
        typeChecker.check();

        if (typeChecker.errorOccurred()) {
            ERROR("One or more errors occurred during type checking, exiting");
            return;
        }

        HEADER("Successful Build, Running...");

        phase++;
        
        auto interpreter = Interpreter(tree);
        BuiltinImplementations::interpreter = interpreter;
        interpreter.run();

        if (interpreter.errorOccurred()) {
            ERROR("One or more errors occurred at runtime, exiting");
            return;
        }
    } catch (...) {
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

        ERROR(errorExitMessage);
    }
}

int main(int argc, char ** argv) {
    std::string sourceStream;

    Logger::getInstance();

    if (argc == 1) {
        ERROR("Error: Source file required");
        exit(1);
    } else if (argc == 2) {
        sourceStream = Lexer::readFile(std::string(argv[1]));
    } else if (argc == 3) {
        if (std::string(argv[1]) == "-d") {
            Logger::getInstance().setLevel(DEBUG);
            sourceStream = Lexer::readFile(std::string(argv[2]));
        } else {
            sourceStream = Lexer::readFile(std::string(argv[1]));
            if (std::string(argv[1]) == "-d") {
                Logger::getInstance().setLevel(DEBUG);
            } else {
                ERROR(std::string("Unknown argument: ") + std::string(argv[1]));
                exit(1);
            }
        }
    }

    if (sourceStream.empty())
        exit(2);
    
    runBant(sourceStream);
}