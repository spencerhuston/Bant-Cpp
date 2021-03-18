#include <string>
#include <iostream>
#include <sstream>

#include "../includes/lexer.hpp"
#include "../includes/parser.hpp"
#include "../includes/typeChecker.hpp"
#include "../includes/interpreter.hpp"

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
    
    //try {
        Format::printHeader("Building...");

        auto lexer = Lexer(BuiltinDefinitions::builtinDefinitions + sourceStream);
        auto tokenStream = lexer.makeTokenStream();

        if (lexer.errorOccurred()) {
            Format::printError("One or more errors occurred during lexing, exiting");
            exit(3);
        }

        auto parser = Parser(tokenStream);
        auto tree = parser.makeTree();

        if (parser.errorOccurred()) {
            Format::printError("One or more errors occurred during parsing, exiting");
            exit(4);
        }

        auto typeChecker = TypeChecker(tree);
        typeChecker.check();

        if (typeChecker.errorOccurred()) {
            Format::printError("One or more errors occurred during type checking, exiting");
            exit(5);
        }

        Format::printHeader("Successful Build, Running...");
        
        auto interpreter = Interpreter(tree);
        interpreter.run();

        if (interpreter.errorOccurred()) {
            Format::printError("One or more errors occurred at runtime, exiting");
            exit(6);
        }
    //} catch (...) {
    //    Format::printError("Unexpected error occurred, exiting");
    //    exit(7);
    //}
}