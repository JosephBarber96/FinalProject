#include <ctime>
#include <math.h>
#include <random>

#include "FastNoise.h"

#include "PopulationMap.h"

PopulationMap::PopulationMap() {}

PopulationMap::~PopulationMap() {}

void PopulationMap::Generate(int size)
{
	// Create noise
	FastNoise fn;

	// Seed
	SeedNoise(fn);

	// Set values
	fn.SetNoiseType(FastNoise::ValueFractal);
	fn.SetFrequency(0.01); // Default 0.01
	fn.SetFractalOctaves(6); // Default 3
	fn.SetInterp(FastNoise::Interp::Quintic); // Default quintic

											  // Fill noise
	FillNoise(fn, size);

	// Positive the values
	ShiftAllValuesPositive();
}

float PopulationMap::GetValueAtPoint(int x, int y)
{
	return popMap[y][x];
}

/* Private methods */

void PopulationMap::SeedNoise(FastNoise& fn)
{
	srand(time(NULL));

	fn.SetSeed(rand() % INT_MAX);
}

void PopulationMap::FillNoise(FastNoise& fn, int size)
{
	for (int y = 0; y < size; y++)
	{
		popMap.push_back(std::vector<float>());
		for (int x = 0; x < size; x++)
		{
			popMap[y].push_back(fn.GetNoise(x, y));
		}
	}
}

void PopulationMap::ShiftAllValuesPositive()
{
	// Find the lowest and highest values
	float highest = INT_MIN;
	float lowest = INT_MAX;
	for (auto vec : popMap)
	{
		for (auto val : vec)
		{
			if (val > highest) highest = val;
			if (val < lowest) lowest = val;
		}
	}

	highestValue = highest;

	// Add the absolute of the lowest onto every value
	// so it ranges from 0-highest+abs(lowest) and not -1 - 1
	float toAdd = fabsf(lowest);
	highestValue += toAdd;
	for (auto &vec : popMap)
	{
		for (auto &val : vec)
		{
			val += toAdd;
		}
	}
}