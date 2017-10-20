/**
* arguments_parser.h
* Purpose: parse arguments supplied from command line
*
* @author iain
* @author Edward Drozdov
* @version 1.0d 20/10/17
*/

#pragma once
#include <string>
#include <vector>


class InputParser {
public:
	InputParser(int &argc, char **argv);
	/// @author iain
	const std::string& getCmdOption(const std::string &option) const {
		std::vector<std::string>::const_iterator itr;
		itr = std::find(this->tokens.begin(), this->tokens.end(), option);
		if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
			return *itr;
		}
		static const std::string empty_string("");
		return empty_string;
	}
	/// @author iain
	bool cmdOptionExists(const std::string &option) const {
		return std::find(this->tokens.begin(), this->tokens.end(), option)
			!= this->tokens.end();
	}
private:
	std::vector <std::string> tokens;
};