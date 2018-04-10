#pragma once

#include <vector>

#include "Road.h"

class BuildingLot;
class MinorRoad : public Road
{
public:
	MinorRoad();
	MinorRoad(int _sx, int _sy, int _ex, int _ey);
	MinorRoad(Vec2* s, Vec2* e);
	~MinorRoad();

	bool markedForDeletion = false;

	void GenerateBuildingLots();

	bool operator==(MinorRoad rhs);

	std::vector<MinorRoad*> endChildren;
	std::vector<MinorRoad*> startChildren;
	std::vector<BuildingLot*> lots;
};