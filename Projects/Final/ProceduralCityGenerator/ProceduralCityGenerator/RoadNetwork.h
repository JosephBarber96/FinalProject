#pragma once

#include <vector>

class RoadNode;
class MinorRoad;
class MajorRoad;
class MinimumSpanningTree;
class WaterData;
class RoadNetwork
{
public:
	RoadNetwork();
	~RoadNetwork();

	float maxMinorRoadDist = 100;

	void GenerateMajorRoads(MinimumSpanningTree* mst, int offsetForRoadNodes);
	void ValidateRoads(WaterData* wd);
	void GenerateBuildingLots();
	void ValidateBuildingLots();
	void GenerateBuildings();

	std::vector<MinorRoad*> minorRoads;
	std::vector<MajorRoad*> majorRoads;
	std::vector<std::vector<RoadNode*>> roadNodes;
};