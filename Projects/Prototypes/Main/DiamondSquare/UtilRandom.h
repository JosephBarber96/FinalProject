#pragma once
#include <random>
class UtilRandom
{
public:
	UtilRandom();
	~UtilRandom();

	std::mt19937 mt;

	int RandomInt(int min, int max);
	float RandomFloat(float min, float max);
	static UtilRandom* Instance();	

private:
	static UtilRandom* instance;
};

