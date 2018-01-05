#include "WaterData.h"

WaterData::WaterData(int size)
{
	fillWaterMap(size);
}

WaterData::~WaterData() {}

void WaterData::SetPixelWater(int x, int y, bool water)
{
	waterMap[x][y] = water;
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