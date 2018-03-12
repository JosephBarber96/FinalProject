#pragma once
#include <vector>
class DiamondSquare;
class WaterData
{
public:
	WaterData(int size);
	~WaterData();

	void LoadFromTerrain(DiamondSquare* terrain, float percentage);
	bool IsWater(int x, int y);
	bool SectionContainsLand(int xOrigin, int width, int yOrigin, int height);

private:
	void SetPixeAsWater(int x, int y, bool water);
	void fillWaterMap(int size);
	std::vector<std::vector<bool>> waterMap;
};