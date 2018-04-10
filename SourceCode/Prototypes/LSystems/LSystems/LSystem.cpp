#include "LSystem.h"
#include "Rule.h"

LSystem::LSystem()
	:
	axiom(""),
	sentence("")
{}

LSystem::LSystem(std::string ax)
	:
	axiom(ax),
	sentence(ax)
{}

LSystem::~LSystem() {}

void LSystem::SetAxiom(std::string ax)
{
	axiom = ax;
	sentence = axiom;
}

bool LSystem::AddRule(char key, std::string output)
{
	Rule* rule = new Rule(key, output);
	return AddRule(rule);
}

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
	if (axiom == "") 
	{ return; }

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