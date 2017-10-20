#include "LSystem.h"
#include "Rule.h"

LSystem::LSystem(std::string ax)
	:
	axiom(ax),
	sentence(ax)
{}

LSystem::~LSystem() {}

bool LSystem::AddRule(Rule* newRule)
{
	for (auto rule : rules)
	{
		if (rule->getInput() == newRule->getInput())
		{
			return false;
		}
	}
	rules.push_back(newRule);
	return true;
}

void LSystem::Generate()
{
	std::string newSentence = "";

	for (auto cha : sentence)
	{
		bool ruleFound = false;
		for (auto rule : rules)
		{
			if (cha == rule->getInput())
			{
				newSentence += rule->getOutput();
				ruleFound = true;
			}
			if (ruleFound) { break; }
		}

		if (!ruleFound)
		{
			newSentence += cha;
		}
	}

	sentence = newSentence;
}