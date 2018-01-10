#pragma once
#include <vector>
class RoadNode;
class BuildingLot;
class Road
{
public:
	Road();
	~Road();

	void GenerateBuildingLots();
	void GenerateBuildingLotsForLongRoad();

	std::vector<RoadNode*> nodes;
	std::vector<BuildingLot*> lots;
};