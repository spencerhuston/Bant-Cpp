#include <string>
#include <iostream>
#include <sstream>

#include "core/lexer/lexer.hpp"
#include "core/parser/parser.hpp"
#include "core/typeChecker/typeChecker.hpp"
#include "core/cpsConverter/cpsConverter.hpp"
#include "core/interpreter/interpreter.hpp"
#include "core/builtin/builtinImplementations.hpp"

#include "utils/logger.hpp"

void
displayExceptionError(const int & phase) {
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

void
runBant(const std::string & sourceStream, const bool & runWithBuiltins) {
    int phase = 0;
    try {
        HEADER("Building...");

        std::string lexerStream{sourceStream};

        if (runWithBuiltins) {
            lexerStream = BuiltinDefinitions::builtinDefinitions + lexerStream;
        }

        auto lexer = Lexer(lexerStream);
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

        auto cpsConverter = CPSConverter(tree);
        cpsConverter.convert();

        HEADER("Successful Build, Running...");

        phase++;
        
        auto interpreter = Interpreter(tree);
        BuiltinImplementations::interpreter = interpreter;
        interpreter.run();

        if (interpreter.errorOccurred()) {
            ERROR("One or more errors occurred at runtime, exiting");
            return;
        }
    } catch (HaltException & haltException) {
        return;
    } catch (RuntimeException & runtimeException) {
        ERROR("Exiting.");
        return;
    } catch (std::runtime_error & runtimeError) {
        displayExceptionError(phase);
    } catch (std::logic_error & logicError) {
        displayExceptionError(phase);
    }
}

char *
getCmdOption(char ** begin, char ** end, const std::string & option) {
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        return *itr;
    }
    return 0;
}

bool
cmdOptionExists(char ** begin, char ** end, const std::string & option) {
    return std::find(begin, end, option) != end;
}

int
main(int argc, char ** argv) {
    std::string sourceStream;

    Logger::getInstance();

    bool runWithBuiltins = true;
    std::string filePath;
    if (argc == 1) {
        ERROR("Error: Source file required");
        exit(1);
    }
    
    if (cmdOptionExists(argv, argv + argc, "-d")) { // Debug
        Logger::getInstance().setLevel(DEBUG);
    }
    
    if (cmdOptionExists(argv, argv + argc, "-nb")) { // No Builtins
        runWithBuiltins = false;
    }
    
    if (cmdOptionExists(argv, argv + argc, "-f")) { // File Path
        filePath = std::string(getCmdOption(argv, argv + argc, "-f"));
    }

    if (filePath.empty()) {
        ERROR("Error: Source file required");
        exit(2);
    }

    sourceStream = Lexer::readFile(filePath);

    if (sourceStream.empty())
        exit(3);
    
    runBant(sourceStream, runWithBuiltins);
}