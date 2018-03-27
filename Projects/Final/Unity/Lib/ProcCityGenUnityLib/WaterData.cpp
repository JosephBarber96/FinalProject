#include <iostream>

#include "WaterData.h"
#include "DiamondSquare.h"
#include "Vec3.h"

WaterData::WaterData(int size)
{
	fillWaterMap(size);
}

WaterData::~WaterData() {}

void WaterData::LoadFromTerrain(DiamondSquare* terrain, float percentage)
{
	// Remember the percentage
	waterPercent = percentage;

	// First, make all of the values positive
	float lowest = terrain->Lowest();
	float highest = terrain->Highest();
	float toAdd = fabsf(lowest);
	for (auto &row : terrain->Points())
	{
		for (auto &point : row)
		{
			point->z += toAdd;
		}
	}

	// Find the new highest and lowest values
	float newLowest = lowest + toAdd;
	float newHighest = highest + toAdd;

	// Find the difference
	float difference = newHighest - newLowest;

	// The bottom 20% shal be considered water
	waterLevel = difference * (percentage / 100);
	std::cout << "Highest terrain point: " << newHighest << std::endl;
	std::cout << "Cut off point: " << waterLevel << std::endl;

	// Get the size
	int wid, hei;
	wid = terrain->getDivisions();
	hei = terrain->getDivisions();

	// Loop through, assign water or terrain
	auto points = terrain->Points();
	for (int y = 0; y < hei; y++)
	{
		for (int x = 0; x < wid; x++)
		{
			// If the Z point is lower than or equal to the cut off point, set it as water. Otherwise, don't
			points[y][x]->z <= waterLevel ? SetPixeAsWater(x, y, true) : SetPixeAsWater(x, y, false);
		}
	}
}

bool WaterData::IsWater(int x, int y)
{
	// OOB
	if (y > waterMap.size() - 1 || x > waterMap[0].size() - 1) return false;

	return (waterMap[x][y] == true);
}

bool WaterData::SectionContainsLand(int xOrigin, int width, int yOrigin, int height)
{
	for (int y = yOrigin; y < yOrigin + height; y++)
	{
		for (int x = xOrigin; x < xOrigin + width; x++)
		{
			// If we found a section that isn't water, return true
			if (!IsWater(x, y)) return true;
		}
	}
	// Return false if none is found
	return false;
}

// Private

void WaterData::SetPixeAsWater(int x, int y, bool water)
{
	waterMap[x][y] = water;
}

void WaterData::fillWaterMap(int size)
{
	for (int y = 0; y < size; y++)
	{
		waterMap.push_back(std::vector<bool>());
		for (int x = 0; x < size; x++)
		{
			waterMap[y].push_back(false);
		}
	}
}