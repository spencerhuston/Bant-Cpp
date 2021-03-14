#pragma once

#include <iostream>
#include <string>

namespace Format {
    static const std::string header = "==============================";

    inline void 
    printHeader(const std::string & title) {
        std::cout << header << std::endl << 
                     title  << std::endl << 
                     header << std::endl; 
    }

    inline void
    printError(const std::string errorString) {
        std::cout << "\033[1;31m" << errorString << "\033[0m" << std::endl;
    }
}