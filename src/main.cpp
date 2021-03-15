#include <string>
#include <iostream>
#include <sstream>

#include "../includes/parser.hpp"
#include "../includes/lexer.hpp"

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
    
    try {
        Format::printHeader("Building...");
        auto lexer = Lexer(std::move(sourceStream));
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

        // Type checking phase

        Format::printHeader("Running...");
        // Interpreter phase
    } catch (...) {
        Format::printError("Unexpected error occurred");
        exit(7);
    }
}