#pragma once
#include <string>
class Rule
{
public:
	Rule(char in, std::string out);
	~Rule();

	char getInput() { return input; }
	std::string getOutput() { return output; }

private:
	char input;
	std::string output;
};