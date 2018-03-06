#pragma once

#include <vector>

#include <boost\polygon\voronoi.hpp>

using namespace boost::polygon;
typedef int coordinate_type;
typedef point_data<coordinate_type> point_type;
typedef voronoi_diagram<double> VD;

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

	void GenerateMinorRoads(VD* voronoi);
	void GenerateMajorRoads(MinimumSpanningTree* mst, int offsetForRoadNodes);
	void ValidateRoads(WaterData* wd);
	void GenerateBuildingLots();
	void ValidateBuildingLots();
	void GenerateBuildings();

	std::vector<MinorRoad*> minorRoads;
	std::vector<MajorRoad*> majorRoads;
	std::vector<std::vector<RoadNode*>> roadNodes;
};