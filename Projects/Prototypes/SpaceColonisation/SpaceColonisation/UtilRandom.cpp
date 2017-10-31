#include "UtilRandom.h"

UtilRandom* UtilRandom::instance;

UtilRandom::UtilRandom() {}

UtilRandom::~UtilRandom() {}

int UtilRandom::Random(int min, int max)
{
	std::uniform_int_distribution<int> distribution(min, max);

	return (distribution(generator));
}

UtilRandom* UtilRandom::Instance()
{
	if (instance == nullptr)
	{
		instance = new UtilRandom();
	}
	return instance;
}