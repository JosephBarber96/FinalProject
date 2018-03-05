#include "City.h"

#include "UtilRandom.h"
#include "Vec2.h"

#include "PopulationMap.h"
#include "DiamondSquare.h"
#include "WaterData.h"
#include "PopulationQuadTree.h"


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
}

void City::Draw()
{

}

/* Private methods */

void City::GeneratePopulationMap()
{
	populationMap = new PopulationMap();
	populationMap->Generate(winSize);
}

void City::GenerateTerrain()
{
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
	waterData = new WaterData(winSize);
	waterData->LoadFromTerrain(terrain, 10);
}

void City::GenerateQuadTree()
{
	quadTree = new PopulationQuadTree(0, 0, winSize, winSize, nullptr, populationMap, waterData, winSize);
}

void City::GenerateVoronoi()
{
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

}