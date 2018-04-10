#pragma once
#include <vector>
class WaterData
{
public:
	WaterData(int size);
	~WaterData();

	void SetPixelWater(int x, int y, bool water);
	bool IsWater(int x, int y);
	bool SectionContainsLand(int xOrigin, int width, int yOrigin, int height);

private:
	void fillWaterMap(int size);
	std::vector<std::vector<bool>> waterMap;
};

