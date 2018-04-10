#pragma once
#include <random>
class UtilRandom
{
public:
	UtilRandom();
	~UtilRandom();

	std::mt19937 mt;

	int Random(int min, int max);
	static UtilRandom* Instance();

private:
	static UtilRandom* instance;
};