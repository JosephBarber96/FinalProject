#pragma once

#include <vector>

#include <boost\polygon\voronoi.hpp>

using namespace boost::polygon;
typedef int coordinate_type;
typedef point_data<coordinate_type> point_type;
typedef voronoi_diagram<double> VD;

class PopulationMap;
class DiamondSquare;
class WaterData;
class PopulationQuadTree;
class MinorRoad;
class MinimumSpanningTree;
class City
{
public:
	City();
	~City();

	void Generate();
	void Draw();

private:
	// Vars
	const int winSize = 512;
	int offsetForRoadNodes = 5;
	float maxMinorRoadDist = 100;

	// City components
	PopulationMap* populationMap;
	DiamondSquare* terrain;
	WaterData* waterData;
	PopulationQuadTree* quadTree;
	VD voronoi;
	std::vector<MinorRoad*> minorRoads;
	MinimumSpanningTree* mst;


private:
	void GeneratePopulationMap();
	void GenerateTerrain();
	void GenerateWaterBoundary();
	void GenerateQuadTree();
	void GenerateVoronoi();
	void GenerateMinorRoads();
	void GenerateMST();
};