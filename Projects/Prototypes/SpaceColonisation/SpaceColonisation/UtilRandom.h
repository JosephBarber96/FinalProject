#pragma once
#include <random>
class UtilRandom
{
public:
	UtilRandom();
	~UtilRandom();

	std::default_random_engine generator;

	int Random(int min, int max);
	static UtilRandom* Instance();	

private:
	static UtilRandom* instance;
};

