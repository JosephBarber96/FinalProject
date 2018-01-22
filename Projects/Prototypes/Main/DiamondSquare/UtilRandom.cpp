#include "UtilRandom.h"

UtilRandom* UtilRandom::instance = 0;

UtilRandom::UtilRandom() 
	:
	mt(std::random_device()())
{}

UtilRandom::~UtilRandom() {}

int UtilRandom::RandomInt(int min, int max)
{
	std::uniform_int_distribution<int> distribution(min, max);

	return (distribution(mt));
}

float UtilRandom::RandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> distribution(min, max);

	return (float)(distribution(mt));
}

UtilRandom* UtilRandom::Instance()
{
	if (instance == nullptr)
	{
		instance = new UtilRandom();
	}
	return instance;
}