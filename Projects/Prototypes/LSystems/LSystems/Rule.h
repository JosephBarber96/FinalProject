#pragma once
#include <string>
class Rule
{
public:
	Rule(char k, std::string out);
	~Rule();

	char getInput() { return key; }
	std::string getOutput() { return output; }

private:
	char key;
	std::string output;
};