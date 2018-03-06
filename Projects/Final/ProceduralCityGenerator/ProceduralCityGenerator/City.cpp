#include <iostream>

#include "City.h"

#include "UtilRandom.h"
#include "Utility.h"
#include "Vec2.h"
#include "Pathfinding.h"

#include "PopulationMap.h"
#include "DiamondSquare.h"
#include "WaterData.h"
#include "PopulationQuadTree.h"
#include "RoadNetwork.h"
#include "MinorRoad.h"
#include "MinimumSpanningTree.h"
#include "RoadNode.h"


City::City() {}

City::~City() {}

void City::Generate()
{
	/* Population map */
	GeneratePopulationMap();

	/* Terrain */
	GenerateTerrain();

	/* Water boundary map */
	GenerateWaterBoundary();

	/* Quad tree */
	GenerateQuadTree();

	/* Voronoi */
	GenerateVoronoi();

	/* Minor roads */
	GenerateMinorRoads();

	/* Mst */
	GenerateMST();

	/* Road nodes */
	GenerateRoadNodes();

	/* Major roads */
	GenerateMajorRoads();

	/* Validate roads */
	ValidateRoads();

	/* Building lots */
	GenerateBuildingLots();
}

void City::Draw()
{

}

/* Private methods */

void City::GeneratePopulationMap()
{
	std::cout << "Generating population map." << std::endl;
	populationMap = new PopulationMap();
	populationMap->Generate(winSize);
}

void City::GenerateTerrain()
{
	std::cout << "Generating terrain." << std::endl;
	int divisions = winSize;
	int size = 5;
	int height = 65;
	terrain = new DiamondSquare(divisions, size, height);
	terrain->Generate();
	terrain->CreatePoints();
	terrain->CalcuateBoundaryPoints();
}

void City::GenerateWaterBoundary()
{
	std::cout << "Generating water." << std::endl;
	waterData = new WaterData(winSize);
	waterData->LoadFromTerrain(terrain, 10);
}

void City::GenerateQuadTree()
{
	std::cout << "Creating QuadTree." << std::endl;
	quadTree = new PopulationQuadTree(0, 0, winSize, winSize, nullptr, populationMap, waterData, winSize);
}

void City::GenerateVoronoi()
{
	std::cout << "Generating voronoi." << std::endl;
	// To hold voronoiPoints
	std::vector<point_type> voronoiPoints;
	int bufferSpace = 5;

	// For each quad in the quad tree
	for (auto quad : quadTree->AllChildren())
	{
		// Get the boundaries of this quad
		int minX = quad->XOrigin();;
		int maxX = quad->XOrigin() +quad->Width();
		int minY = quad->YOrigin();
		int maxY = quad->YOrigin() + quad->Height();

		// Do not place a point in a quad that contains water..

		// Find all valid land positions in this quad
		std::vector<Vec2> waterPositions;
		for (int y = minY; y < maxY; y++)
		{
			for (int x = minX; x < maxX; x++)
			{
				if (waterData->IsWater(x, y))
				{
					waterPositions.push_back(Vec2(x, y));
				}
			}
		}
		// If none exist, continue without placing a point
		if (waterPositions.size() > 0) { continue; }

		// Otherwise, place a point
		double px = UtilRandom::Instance()->RandomFloat(minX + bufferSpace, maxX - bufferSpace);
		double py = UtilRandom::Instance()->RandomFloat(minY + bufferSpace, maxY - bufferSpace);

		voronoiPoints.push_back(point_type(px, py));
	}

	// Construct the voronoi diagram
	VD vd;
	construct_voronoi(voronoiPoints.begin(), voronoiPoints.end(), &vd);
}

void City::GenerateMinorRoads()
{
	std::cout << "Creating minor roads." << std::endl;
	roadNetwork = new RoadNetwork();

	//minorRoads
	for (auto const &edge : voronoi.edges())
	{
		if (edge.vertex0() != NULL && edge.vertex1() != NULL)
		{
			int sX = edge.vertex0()->x();
			int sY = edge.vertex0()->y();

			int eX = edge.vertex1()->x();
			int eY = edge.vertex1()->y();


			// See if this exists already
			bool exists = false;
			for (auto r : roadNetwork->minorRoads)
			{
				if (r->Equals(sX, sY, eX, eY))
				{
					exists = true;
					break;
				}
			}
			if (exists) continue;

			// If not, create
			MinorRoad* road = new MinorRoad(sX, eY, eX, eY);
			roadNetwork->minorRoads.push_back(road);
		}
	}
}

void City::GenerateMST()
{
	std::cout << "Generating Minimum spanning tree." << std::endl;
	mst = new MinimumSpanningTree();

	// Spawn a point in each quad
	for (auto quad : quadTree->AllChildren())
	{
		if (static_cast<PopulationQuadTree*>(quad)->Indexer() > 2)
		{
			mst->SpawnPoint(waterData, 
				offsetForRoadNodes, 
				quad->XOrigin(), 
				quad->YOrigin(), 
				(quad->XOrigin() + quad->Width()), 
				(quad->YOrigin() + quad->Height()));
		}
	}

	mst->AssignNeighbours(winSize / 4);
	mst->CreateEdges();
	mst->Sort();
}

void City::GenerateRoadNodes()
{
	std::cout << "Generating road nodes." << std::endl;
	roadNetwork->roadNodes = terrain->CreatePointsAndPassBackRoadNodes(offsetForRoadNodes, waterData);
}

void City::GenerateMajorRoads()
{
	std::cout << "Generating major roads." << std::endl;
	roadNetwork->GenerateMajorRoads(mst, offsetForRoadNodes);
}

void City::ValidateRoads()
{
	std::cout << "Validating roads." << std::endl;
	roadNetwork->ValidateRoads(waterData);
}

void City::GenerateBuildingLots()
{
	std::cout << "Generating building lots." << std::endl;
	roadNetwork->GenerateBuildingLots();
	roadNetwork->ValidateBuildingLots();
}