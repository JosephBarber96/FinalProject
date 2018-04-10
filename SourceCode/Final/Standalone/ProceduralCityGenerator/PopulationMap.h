#pragma once

#include <vector>

class FastNoise;
class PopulationMap
{
public:
	PopulationMap();
	~PopulationMap();

	void Generate(int size);

	float GetValueAtPoint(int x, int y);
	float GetHighestValue() const { return highestValue; }

	std::vector<std::vector<float>> GetPopulationMap() const { return popMap; }

private:
	void SeedNoise(FastNoise& fn);
	void FillNoise(FastNoise& fn, int size);
	void ShiftAllValuesPositive();

	std::vector<std::vector<float>> popMap;
	float highestValue;
};