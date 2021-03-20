#include "../../includes/utils/filePosition.hpp"

FilePosition::FilePosition(const int line, const int column, const std::string & text) 
: fileLine(line), fileColumn(column), currentLineText(text) { }