#include "UtilRandom.h"

UtilRandom* UtilRandom::instance = 0;

UtilRandom::UtilRandom() 
	:
	mt(std::random_device()())
{}

UtilRandom::~UtilRandom() {}

int UtilRandom::Random(int min, int max)
{
	std::uniform_int_distribution<int> distribution(min, max);

	return (distribution(mt));
}

UtilRandom* UtilRandom::Instance()
{
	if (instance == nullptr)
	{
		instance = new UtilRandom();
	}
	return instance;
}