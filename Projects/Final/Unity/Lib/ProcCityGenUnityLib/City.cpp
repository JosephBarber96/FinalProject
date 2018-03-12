#include <iostream>

#include "City.h"

#include "UtilRandom.h"
#include "Utility.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Pathfinding.h"

#include "PopulationMap.h"
#include "DiamondSquare.h"
#include "WaterData.h"
#include "PopulationQuadTree.h"
#include "RoadNetwork.h"
#include "MinorRoad.h"
#include "MajorRoad.h"
#include "MinimumSpanningTree.h"
#include "RoadNode.h"
#include "BuildingLot.h"
#include "MstEdge.h"
#include "MstNode.h"
#include "FloorPlan.h"
#include "shapeLine.h"


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

	/* Buildings */
	GenerateBuildings();
}

/* Private methods */

void City::GeneratePopulationMap()
{
	std::cout << "Generating population map." << std::endl;
	populationMap = new PopulationMap();
	populationMap->Generate(citySize);
}

void City::GenerateTerrain()
{
	std::cout << "Generating terrain." << std::endl;
	int divisions = citySize;
	int size = 5;
	int height = 64;
	terrain = new DiamondSquare(divisions, size, height);
	terrain->Generate();
	terrain->CreatePoints();
	terrain->CalcuateBoundaryPoints();
}

void City::GenerateWaterBoundary()
{
	std::cout << "Generating water." << std::endl;
	waterData = new WaterData(citySize);
	waterData->LoadFromTerrain(terrain, 10);
}

void City::GenerateQuadTree()
{
	std::cout << "Creating QuadTree." << std::endl;
	quadTree = new PopulationQuadTree(0, 0, citySize, citySize, nullptr, populationMap, waterData, citySize);
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
		int minX = quad->XOrigin();
		int maxX = quad->XOrigin() + quad->Width();
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
	construct_voronoi(voronoiPoints.begin(), voronoiPoints.end(), &voronoi);
}

void City::GenerateMinorRoads()
{
	std::cout << "Creating minor roads." << std::endl;
	roadNetwork = new RoadNetwork();
	roadNetwork->GenerateMinorRoads(&voronoi);

	std::cout << "Minor roads created: " << roadNetwork->minorRoads.size() << std::endl;
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

	mst->AssignNeighbours(citySize / 4);
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

void City::GenerateBuildings()
{
	std::vector<Vec2*> lotCenter;
	std::vector<Vec2*> buildingCenter;

	for (auto road : roadNetwork->minorRoads)
	{
		for (auto lot : road->lots)
		{
			// Give the lot a new FloorPlan object
			lot->fp = new FloorPlan();

			// Create the lots bounding box for building generation
			lot->fp->SetBoundingBox(lot->minX, lot->minY, lot->maxX - lot->minX, lot->maxY - lot->minY);

			// Generate shapes
			int numberOfShapes = UtilRandom::Instance()->RandomInt(2, 5);
			lot->fp->GenerateShapes(numberOfShapes);

			// Calculate building perimeter
			lot->fp->GeneratePerimeter();

			// Find the center point of the generated building

			// First we need to find the minimum and maximum boundaries
			float buildingMinX = FLT_MAX, buildingMaxX = -FLT_MAX, buildingMinY = FLT_MAX, buildingMaxY = -FLT_MAX;
			for (shapeLine* line : lot->fp->perimeterLines)
			{
				if (line->start->x < buildingMinX) buildingMinX = line->start->x;
				if (line->start->x > buildingMaxX) buildingMaxX = line->start->x;

				if (line->start->y < buildingMinY) buildingMinY = line->start->y;
				if (line->start->y > buildingMaxY) buildingMaxY = line->start->y;

				if (line->end->x < buildingMinX) buildingMinX = line->start->x;
				if (line->end->x > buildingMaxX) buildingMaxX = line->start->x;

				if (line->end->y < buildingMinY) buildingMinY = line->start->y;
				if (line->end->y > buildingMaxY) buildingMaxY = line->start->y;
			}

			// Then we find the center
			float buildingDiffX = buildingMaxX - buildingMinX;
			float buildingDiffY = buildingMaxY - buildingMinY;

			float buildingMidX = buildingMaxX - (buildingDiffX / 2);
			float buildingMidY = buildingMaxY - (buildingDiffY / 2);


			/*
			Find the angle to rotate by
			*/

			// Find the roads facing angle
			Vec2 parentDir = Vec2(
				road->End()->x - road->Start()->x,
				road->End()->y - road->Start()->y);
			float parentAngle = Vec2::VectorToAngle(parentDir);

			float angleToRotate = 90 - parentAngle - 45;

			// Rotate the points around the found center point
			for (shapeLine* line : lot->fp->perimeterLines)
			{
				line->start = Utility::RotateAroundPoint(line->start, new Vec2(buildingMidX, buildingMidY), angleToRotate);
				line->end = Utility::RotateAroundPoint(line->end, new Vec2(buildingMidX, buildingMidY), angleToRotate);
			}

			/*
			Move the building so the center point of the building is the center point of the shape
			*/

			// Find the center of the lot
			float lotDiffX = lot->maxX - lot->minX;
			float lotDiffY = lot->maxY - lot->minY;

			float lotMidX = lot->maxX - (lotDiffX / 2);
			float lotMidY = lot->maxY - (lotDiffY / 2);

			// Find out the offset
			float offsetX = lotMidX - buildingMidX;
			float offsetY = lotMidY - buildingMidY;

			buildingCenter.push_back(new Vec2(buildingMidX, buildingMidY));
			lotCenter.push_back(new Vec2(lotMidX, lotMidY));

			// Move the lines by this much
			for (shapeLine* line : lot->fp->perimeterLines)
			{
				line->start->x += offsetX;
				line->start->y += offsetY;

				line->end->x += offsetX;
				line->end->y += offsetY;
			}
		}
	}
}