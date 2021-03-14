#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include "../includes/parser.hpp"
#include "../includes/lexer.hpp"

const std::string
getSourceStream(const std::string sourceFileName) {
    std::ifstream sourceFile(sourceFileName);
    
    std::string sourceStream;
    if (sourceFile.is_open()) {
        std::ostringstream sourceFileStream;
        sourceFileStream << sourceFile.rdbuf();
        sourceStream = sourceFileStream.str();
    } else
        std::cout << "Could not open file: " << sourceFileName << std::endl;

    return sourceStream;
}

int main(int argc, char ** argv) {
    if (argc == 1) {
        std::cout << "Error: Source file required" << std::endl;
        exit(1);
    }

    auto sourceStream = getSourceStream(std::string(argv[1]));
    if (sourceStream.empty())
        exit(2);
    
    auto lexer = Lexer(std::move(sourceStream));
    auto tokenStream = lexer.makeTokenStream();

    if (lexer.errorOccurred()) {
        Format::printError("One or more errors occurred during lexing, exiting");
        exit(3);
    }

    auto parser = Parser(tokenStream);
}