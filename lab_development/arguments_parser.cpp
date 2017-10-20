/**
* arguments_parser.h
* Purpose: parse arguments supplied from command line
*
* @author iain
* @author Edward Drozdov
* @version 1.0d 20/10/17
*/

#include "arguments_parser.h"


InputParser::InputParser(int &argc, char **argv) {
	for (int i = 1; i < argc; ++i)
		this->tokens.push_back(std::string(argv[i]));
};