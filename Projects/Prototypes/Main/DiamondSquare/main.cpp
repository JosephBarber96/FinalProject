#include <vector>
#include <array>
#include <iostream>
#include <math.h>

/*
	Requires C++ Boost Library for 1.65s for windows
	https://dl.bintray.com/boostorg/release/1.65.1/binaries/
	(boost_1_65_1-msvc-10.0-32.exe)

	include directory: C:\local\boost_1_65_1
*/
#include <boost\polygon\voronoi.hpp>
#include "FastNoise.h"
#include <SFML\Graphics.hpp>

#include "DiamondSquare.h"
#include "QuadTree.h"
#include "MinimumSpanningTree.h"
#include "RoadNode.h"
#include "Node.h"
#include "MstNode.h"
#include "V2.h"
#include "Edge.h"
#include "Road.h"
#include "Pathfinding.h"
#include "WaterData.h"
#include "BuildingLot.h"
#include "UtilRandom.h"
#include "Utility.h"
#include "Line.h"

#include "FloorPlan.h"
#include "BoundingBox.h"
#include "jbShape.h"
#include "shapeLine.h"

//! Voronoi
using namespace std;
using namespace boost::polygon;
typedef int coordinate_type;
typedef point_data<coordinate_type> point_type;
typedef voronoi_diagram<double> VD;

const int winSize = 512;
int offsetForRoadNodes = 5;
float maxMinorRoadDist = 100;

template <typename Iter>
Iter ReturnNextIter(Iter iter)
{
	return ++iter;
}

void makeAllFastNoiseValuesPositive(std::vector<std::vector<float>> &popMap, float &highestVal)
{
	// Find the lowest and highest values
	float highest = INT_MIN;
	float lowest = INT_MAX;
	for (auto vec : popMap)
	{
		for (auto val : vec)
		{
			if (val > highest) highest = val;
			if (val < lowest) lowest = val;
		}
	}

	highestVal = highest;

	// Add the absolute of the lowest onto every value
	// so it ranges from 0-highest+abs(lowest) and not -1 - 1
	float toAdd = fabsf(lowest);
	highestVal += toAdd;
	for (auto &vec : popMap)
	{
		for (auto &val : vec)
		{
			val += toAdd;
		}
	}

}

void seedNoise(FastNoise &fn)
{
	srand(time(NULL));

	fn.SetSeed(rand() % INT_MAX);
}

void fillNoise(FastNoise &fn, std::vector<std::vector<float>> &popMap, int size)
{
	for (int y = 0; y < size; y++)
	{
		popMap.push_back(std::vector<float>());
		for (int x = 0; x < size; x++)
		{
			popMap[y].push_back(fn.GetNoise(x, y));
		}
	}
}

void LoadWaterDataFromMap(WaterData &wd)
{
	sf::Image waterMap;
	waterMap.loadFromFile("watermap.bmp");

	int wid = waterMap.getSize().x;
	int hei = waterMap.getSize().y;

	for (int y = 0; y < hei; y++)
	{
		for (int x = 0; x < wid; x++)
		{
			waterMap.getPixel(x, y) == sf::Color::White ? wd.SetPixelWater(x, y, true) : wd.SetPixelWater(x, y, false);
		}
	}
}

void LoadWaterDataFromTerrain(WaterData &wd, DiamondSquare terrain, float percentage)
{
	// First, make all of the values positive
	float lowest = terrain.Lowest();
	float highest = terrain.Highest();
	float toAdd = fabsf(lowest);
	for (auto &row : terrain.Points())
	{
		for (auto &point : row)
		{
			point->z += toAdd;
		}
	}

	// Find the new highest and lowest values
	float newLowest = lowest + toAdd;
	float newHighest = highest + toAdd;

	// Find the difference
	float difference = newHighest - newLowest;

	// The bottom 20% shal be considered water
	float cutoffPoint = difference * (percentage / 100);
	std::cout << "Highest terrain point: " << newHighest << std::endl;
	std::cout << "Cut off point: " << cutoffPoint << std::endl;

	// Get the size
	int wid, hei;
	wid = terrain.getDivisions();
	hei = terrain.getDivisions();

	// Loop through, assign water or terrain
	auto points = terrain.Points();
	for (int y = 0; y < hei; y++)
	{
		for (int x = 0; x < wid; x++)
		{
			// If the Z point is lower than or equal to the cut off point, set it as water. Otherwise, don't
			points[y][x]->z <= cutoffPoint ? wd.SetPixelWater(x, y, true) : wd.SetPixelWater(x, y, false);
		}
	}
}

int main()
{
	/*************************
		Population noise
	**************************/
	std::cout << "Generation population noise." << std::endl;
	FastNoise fn;
	std::vector<std::vector<float>> popMap;
	seedNoise(fn);
	fn.SetNoiseType(FastNoise::ValueFractal);
	fn.SetFrequency(0.01); // Default 0.01
	fn.SetFractalOctaves(6); // Default 3
	fn.SetInterp(FastNoise::Interp::Quintic); // Default Quintic
	fillNoise(fn, popMap, winSize);

	/* Make the values positive */
	float highestVal;
	makeAllFastNoiseValuesPositive(popMap, highestVal);

	/************************
		Height/terrain data
	*************************/
	std::cout << "Generating DiamondSquare terrain..." << std::endl;
	int divisions = winSize;
	int size = 5;
	int height = 64;
	DiamondSquare terrain = DiamondSquare(divisions, size, height);
	terrain.Generate();
	terrain.CreatePoints();
	terrain.CalcuateBoundaryPoints();
	// ds.Print();

	/************************
		Water-boundary map
	*************************/
	std::cout << "Loading water-boundary map." << std::endl;
	WaterData waterData = WaterData(winSize);
	// LoadWaterDataFromMap(waterData);
	LoadWaterDataFromTerrain(waterData, terrain, 10);

	// To display
	sf::Texture waterMapTexture;
	waterMapTexture.loadFromFile("watermap.bmp");
	waterMapTexture.setSmooth(true);
	sf::Sprite waterMapSprite;
	waterMapSprite.setTexture(waterMapTexture);

	/***************************
			Quad tree
	**************************/
	std::cout << "Creating quad-tree." << std::endl;
	QuadTree* qt = new QuadTree(0, 0, winSize, winSize, nullptr, popMap, waterData, winSize, highestVal);

	/****************************
		Voronoi diagram 
		to create minor roads
	*****************************/

	std::cout << "Creating voronoi minor roads." << std::endl;

	// Create voronoi points
	vector<point_type> voronoiPoints;
	int bufferSpace = 5;

	// For each quad in the quadtree
	for (auto quad : qt->GetTreeChildren())
	{
		// Get the boundaries of this quad
		int minX = quad->xOrigin;
		int maxX = quad->xOrigin + quad->width;
		int minY = quad->yOrigin;
		int maxY = quad->yOrigin + quad->height;

		// Do not place a point in a quad that contains water..

		// Find all valid land positions in this quad
		std::vector<V2> waterPositions;
		for (int y = minY; y < maxY; y++)
		{
			for (int x = minX; x < maxX; x++)
			{
				if (waterData.IsWater(x, y))
				{
					waterPositions.push_back(V2(x, y));
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

	// Create major roads from voronoi edges
	std::vector<Road*> minorRoads;
	for (auto const &edge : vd.edges())
	{
		if (edge.vertex0() != NULL && edge.vertex1() != NULL)
		{
			int sX = edge.vertex0()->x();
			int sY = edge.vertex0()->y();

			int eX = edge.vertex1()->x();
			int eY = edge.vertex1()->y();

			Road* road = new Road(sX, sY, eX, eY);

			bool exists = false;
			for (auto r : minorRoads)
			{
				if (*road->start == *r->end && *road->end == *r->start)
				{
					exists = true;
					break;
				}
			}
			if (!exists)
			{
				minorRoads.push_back(road);
			}
		}
	}

	/************************
				MST
	*************************/
	std::cout << "Creating MST..." << std::endl;

	MinimumSpanningTree mst = MinimumSpanningTree();
	for (auto quad : qt->GetTreeChildren())
	{
		// Only spawn quads in the big-enough quads
		if (quad->indexer > 2)
		{
			mst.SpawnPoint(waterData, offsetForRoadNodes, quad->xOrigin, quad->yOrigin, (quad->xOrigin + quad->width), (quad->yOrigin + quad->height));
		}
	}
	float maximumDistanceBetweenNeighbours = winSize / 4;
	mst.AssignNighbours(maximumDistanceBetweenNeighbours); // Assign neighbours
	mst.CreateAllEdges(); // Create edges
	mst.Sort(); // Sort the MST

	/****************************************
		Creating roadnodes from terrain data
	*****************************************/
	std::vector<std::vector<RoadNode*>> roadNodes;
	roadNodes = terrain.CreatePointsAndPassBackRoadNodes(offsetForRoadNodes, waterData);

	/***************************************************
		Pathfind roads using the mst edges
		and roadnodes generated from the diamondsquare
		terrain
	****************************************************/

	std::vector<Road*> majorRoads;
	std::cout << "Pathfinding " << mst.GetTreeEdges().size() << " roads..." << std::endl;
	int roadCounter = 0;
	for (Edge edge : mst.GetTreeEdges())
	{
		if (edge.start->position->x == edge.end->position->x && edge.start->position->y == edge.end->position->y)
		{ 
			continue; 
		}

		// Create the road
		Road* road = new Road();

		// Pathfind to get the nodes
		road->nodes = Pathfinding::PathFind(roadNodes,
			edge.start->position->x / offsetForRoadNodes,
			edge.start->position->y / offsetForRoadNodes,
			edge.end->position->x / offsetForRoadNodes,
			edge.end->position->y / offsetForRoadNodes,
			offsetForRoadNodes);

		// Split the road into segments
		for (std::vector<RoadNode*>::iterator iter = road->nodes.begin(); iter != road->nodes.end(); iter++)
		{
			if (ReturnNextIter(iter) != road->nodes.end())
			{
				road->segments.push_back(
					new Road((*iter)->position, (*ReturnNextIter(iter))->position));
			}
		}

		majorRoads.push_back(road);

		std::cout << "\r" << roadCounter++ << " roads complete.\t\t";
	}
	std::cout << std::endl;

	/*******************************************************
			Remove any voronoi minor roads that
			overlap main mst roads and reconnect the 
			voronoi to the main roads
	********************************************************/
	std::vector<V2*> expandPoints;

	std::cout << "Checking for minor:major road intersections..." << std::endl;
	int minorMajorIntersectionCounter = 0;
	// For each minor road
	for (auto minor : minorRoads)
	{
		// Check each major road
		for (auto &major : majorRoads)
		{
			// Check each segment
			for (auto &seg : major->segments)
			{
				if (Utility::GetIntersectionPointWithFiniteLines(minor->start, minor->end, seg->start, seg->end) != nullptr)
				{
					minorMajorIntersectionCounter++;

					// Mark this road for deletion
					minor->markedForDeletion = true;

					// Tell its neighbours to expand and find a major road to connect to
					expandPoints.push_back(new V2(minor->start->x, minor->start->y));
					expandPoints.push_back(new V2(minor->end->x, minor->end->y));
				}
			}
		}
	}
	std::cout << minorMajorIntersectionCounter << " found. Deleting roads." << std::endl;

	// Remove and minor roads that have been marked for deletion
	for (std::vector<Road*>::iterator iter = minorRoads.begin(); iter != minorRoads.end(); /**/)
	{
		if ((*iter)->markedForDeletion)
		{
			// Tell its neighbours to expand and find a major road to connect to
			iter = minorRoads.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	// Time to reconnect the voronoi to the main roads

	// For each point
	for (V2* point : expandPoints)
	{
		// Find the closest roadNode to it
		RoadNode* closest = nullptr;

		// Check each node
		for (Road* road : majorRoads)
		{
			for (RoadNode* node : road->nodes)
			{
				// The first will be closest by default
				if (closest == nullptr) { closest = node; }

				// If node is closer than closest, closest becomes node
				if (V2::DistanceBetween(*node->position, *point) < V2::DistanceBetween(*closest->position, *point))
				{
					closest = node;
				}
			}
		}

		// After finding the closest, create this new road
		minorRoads.push_back(
			new Road(point->x, 
				point->y, 
				closest->position->x, 
				closest->position->y));
	}

	// Now that we've built new minor roads, we need to check for collisions between any minor roads

	for (Road* minorRoad : minorRoads)
	{
		for (Road* other : minorRoads)
		{
			// Skip itself
			if (*minorRoad == *other) { continue; }

			// if Other is already marked for deletion, skip
			// We don't want both roads to get deleted which would cause there to be 
			// 0 connections instead of 2 (we want there to be 1)
			if (other->markedForDeletion) { continue; }

			// Get the intersection point
			V2* iPoint = Utility::GetIntersectionPointWithFiniteLines(minorRoad->start, minorRoad->end, other->start, other->end);

			// If it's not null
			if (iPoint != nullptr)
			{
				// Make sure its not where the roads naturally connect
				if (*iPoint == *minorRoad->start
					|| *iPoint == *minorRoad->end
					|| *iPoint == *other->start
					|| *iPoint == *other->end)
				{
					continue;
				}
			}
			else { continue; }

			// If not, continue
			minorRoad->markedForDeletion = true;
		}
	}

	// We can also detect here to delete and roads that have an end which sits inside of a body of water
	for (Road* minorRoad : minorRoads)
	{
		if (waterData.IsWater(minorRoad->start->x, minorRoad->start->y)
			||
			waterData.IsWater(minorRoad->end->x, minorRoad->end->y))
		{
			minorRoad->markedForDeletion = true;
		}
	}

	// Next we delete any minor roads that are above a certain length threshold
	// this is to remove the long outwards lines left over from the voronoi
	for (Road* minorRoad : minorRoads)
	{
		if (V2::DistanceBetween(*minorRoad->start, *minorRoad->end) > maxMinorRoadDist)
		{
			minorRoad->markedForDeletion = true;
		}
	}

	// Remove any new minor roads that have been marked for deletion
	for (std::vector<Road*>::iterator iter = minorRoads.begin(); iter != minorRoads.end(); /**/)
	{
		if ((*iter)->markedForDeletion)
		{
			// Erase
			iter = minorRoads.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	// Now that we've deleted all of these roads, neighbour the remaining roads
	for (Road* minorRoad : minorRoads)
	{
		// Against every other road
		for (Road* otherRoad : minorRoads)
		{
			// Skip itself
			if (*minorRoad == *otherRoad) { continue; }

			// Check for neighbours

			// If otherRoad has minorRoad.Start
			if (*minorRoad->start == *otherRoad->start
				||
				*minorRoad->start == *otherRoad->end)
			{
				minorRoad->startChildren.push_back(minorRoad);
			}

			// If otherRoad has minorRoad.End
			if (*minorRoad->end == *otherRoad->start
				||
				*minorRoad->end == *otherRoad->end)
			{
				minorRoad->endChildren.push_back(minorRoad);
			}
		}
	}

	// Remove any minor roads that have 0 neighbours (and thus are sitting along)
	for (std::vector<Road*>::iterator iter = minorRoads.begin(); iter != minorRoads.end(); /**/)
	{
		// no neighbours on BOTH sides, it has to have 0 end AND 0 start
		if ((*iter)->endChildren.size() == 0 && (*iter)->startChildren.size() == 0)
		{
			iter = minorRoads.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	/***********************************************
		Now that the minor roads are set up, 
		generate building lots in each minor 
		road that is still in the road network
	************************************************/
	std::cout << "Generating building lots on minor roads" << std::endl;
	int lotCounter = 0;
	for (Road* road : minorRoads)
	{
		std::cout << "\rGenerating lot of road: " << lotCounter++ << "\t\t";
		road->GenerateBuildingLots();
	}
	std::cout << std::endl;

	/*******************************************************
		Check for and remove any building lot collisions
	********************************************************/

	std::cout << "Checking for building lot collisions" << std::endl;

	/* MINOR ROADS */

	/* First, we prioritive collisions against minor roads and major roads */

	V2* ip;

	// For every building lot
	for (Road* minorRoad : minorRoads)
	{
		for (BuildingLot* lot : minorRoad->lots)
		{
			// Check every minor road
			for (Road* otherRoad : minorRoads)
			{
				// Skip itself
				if (*minorRoad == *otherRoad) { continue; }

				// Check if each line of the lot intersects with the otherRoad
				for (Line* line : lot->GetLotLines())
				{
					ip = Utility::GetIntersectionPointWithFiniteLines(line->start, line->end, otherRoad->start, otherRoad->end);
					if (ip != nullptr)
					{
						lot->markForDeletion = true;
					}
				}
			}
		}
	}

	//int majorRoadsChecked = 0;
	//// For every building lot
	//for (Road* minorRoad : minorRoads)
	//{
	//	for (BuildingLot* lot : minorRoad->lots)
	//	{
	//		// Check every major road
	//		for (Road* majorRoad : majorRoads)
	//		{
	//			std::cout <<"\r" << majorRoadsChecked++ << " major roads checked for intersections \t";
	//			// Check each segment of the major road
	//			for (auto seg : majorRoad->segments)
	//			{
	//				// Check if each line of the lot intersects with the segment
	//				for (Line* line : lot->GetLotLines())
	//				{
	//					if (Utility::GetIntersectionPointWithFiniteLines(line->start, line->end, seg->start, seg->end) != nullptr)
	//					{
	//						lot->markForDeletion = true;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	int minorLotDeletionCounter = 0;
	for (Road* road : minorRoads)
	{
		std::vector<BuildingLot*> lots = road->lots;

		for (std::vector<BuildingLot*>::iterator it = lots.begin(); it != lots.end(); /**/)
		{
			if ((*it)->markForDeletion)
			{
				it = lots.erase(it);
				minorLotDeletionCounter++;
			}
			else
				++it;
		}
		road->lots = lots;
	}


	/* Next, we check for collisions against other building lots */

	// Check every road against every other road
	for (Road* minorRoad : minorRoads)
	{
		for (Road* otherRoad : minorRoads)
		{
			// Do not check the road against itself
			if (*minorRoad == *otherRoad) { continue; }

			// Check each lot of each road
			for (BuildingLot* lot : minorRoad->lots)
			{
				for (BuildingLot* otherLot : otherRoad->lots)
				{
					// If the lots are far away we do not need to check for interceptions
					if (V2::DistanceBetween(*lot->bottomLeft, *otherLot->bottomLeft) > 15)
					{
						continue;
					}

					if (lot->IsLotWithin(otherLot))
					{
						lot->markForDeletion = true;
						otherLot->markForDeletion = true;
					}
				}
			}

		}
	}

	for (Road* road : minorRoads)
	{
		std::vector<BuildingLot*> lots = road->lots;

		for (std::vector<BuildingLot*>::iterator it = lots.begin();  it != lots.end(); /**/)
		{
			if ((*it)->markForDeletion)
			{
				it = lots.erase(it);
				minorLotDeletionCounter++;
			}	
			else
				++it;
		}
		road->lots = lots;
	}

	/* MAJOR ROADS */

	//std::cout << "Checking for lot interceptions" << std::endl;

	//int lotInterceptionCounter = 0;
	//// For every lot
	//for (auto &road : majorRoads) { 
	//	std::cout << "\rChecking road " << lotInterceptionCounter++ << "/" << majorRoads.size() << "\t\t";
	//	for (auto &lot : road->lots) {

	//		// For every other lot
	//		for (auto &otherRoad : majorRoads) {
	//			for (auto &otherLot : road->lots) {

	//				// Don't check a lot against itself
	//				if (*lot == *otherLot) { continue; }

	//				// We don't need to check lots that have already been checked
	//				if (otherLot->markForDeletion) { continue; }

	//				// We don't need to check lots that aren't within the minimum range
	//				if (V2::DistanceBetween(*lot->bottomLeft, *otherLot->bottomLeft) > 10) { continue; }

	//				if (lot->IsLotWithin(otherLot))
	//				{
	//					std::cout << "\nSetting a lot to be marked." << std::endl;
	//					otherLot->markForDeletion = true;
	//				}
	//			}
	//		}
	//	}
	//}
	//std::cout << std::endl;

	//int deletionCounter = 0;
	//int counter = 0;

	//for (auto &road : majorRoads)
	//{
	//	std::vector<BuildingLot*> lots = road->lots;

	//	for (std::vector<BuildingLot*>::iterator it = lots.begin();  it != lots.end(); /**/)
	//	{
	//		counter++;
	//		if ((*it)->markForDeletion)
	//		{
	//			it = lots.erase(it);
	//			deletionCounter++;
	//		}	
	//		else
	//			++it;
	//	}
	//	road->lots = lots;
	//}

	//std::cout << counter << " building lots." << std::endl;
	//std::cout << deletionCounter << " deleted." << std::endl;

	/*******************************************************
		Generate buildings inside of each building lot
	********************************************************/

	std::vector<V2*> lotCenter;
	std::vector<V2*> buildingCenter;

	for (auto road : majorRoads)
	{
		for (auto lot : road->lots)
		{
			// Get the boundaries of the lot
			float lotMinX, lotMinY, lotMaxX, lotMaxY;
			lot->GetOutwardValues(lotMinX, lotMaxX, lotMinY, lotMaxY);

			// Give the lot a new FloorPlan object
			lot->fp = new FloorPlan();

			// Create the lots bounding box for building generation
			lot->fp->SetBoundingBox(lotMinX, lotMinY, lotMaxX - lotMinX, lotMaxY - lotMinY);

			// Generate shapes
			int numberOfShapes = UtilRandom::Instance()->RandomInt(2, 4);
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

			// Get the road the lot belongs to
			Road* lotParent = lot->parent;

			// Get parent start and end
			V2* parentStart = lotParent->nodes[0]->position;
			V2* parentEnd = lotParent->nodes[1]->position;

			// Find the parents facing angle
			V2* parentDir = new V2(parentEnd->x - parentStart->x, parentEnd->y - parentStart->y);
			float parentAngle = V2::VectorToAngle(parentDir);

			float angleToRotate = 90 - parentAngle - 45;

			// Rotate the points around the found center point
			for (shapeLine* line : lot->fp->perimeterLines)
			{
				line->start = Utility::RotateAroundPoint(line->start, new V2(buildingMidX, buildingMidY), angleToRotate);
				line->end = Utility::RotateAroundPoint(line->end, new V2(buildingMidX, buildingMidY), angleToRotate);
			}

			/* 
			Move the building so the center point of the building is the center point of the shape
			*/

			// Find the center of the lot
			float lotDiffX = lotMaxX - lotMinX;
			float lotDiffY = lotMaxY - lotMinY;

			float lotMidX = lotMaxX - (lotDiffX / 2);
			float lotMidY = lotMaxY - (lotDiffY / 2);

			// Find out the offset
			float offsetX = lotMidX - buildingMidX;
			float offsetY = lotMidY - buildingMidY;

			buildingCenter.push_back(new V2(buildingMidX, buildingMidY));
			lotCenter.push_back(new V2(lotMidX, lotMidY));

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

	/* Create window */
	sf::RenderWindow window(sf::VideoMode(winSize, winSize), "Window");
	sf::View view(sf::FloatRect(0, 0, winSize, winSize));
	window.setView(view);

	bool drawPopMap = false,
		drawHeightMap = false,
		drawWaterBoundaryMap = false,
		drawQuadTree = false,
		drawMstNodes = false,
		drawRoads = true,
		drawMST = false,
		drawBuildingLots = false,
		drawBuildings = false,
		drawMinorRoads = false,
		drawVoronoiPointsForMinorRoads = false;

	std::cout << std::endl << "Instructions: " << std::endl;
	std::cout << "\t1: Toggle population map" << std::endl;
	std::cout << "\t2: Toggle height map" << std::endl;
	std::cout << "\t3: Toggle water-boundary map" << std::endl;
	std::cout << "\t4: Toggle quad-tree" << std::endl;
	std::cout << "\t5: Toggle mst nodes" << std::endl;
	std::cout << "\t6: Toggle roads" << std::endl;
	std::cout << "\t7: Toggle MST" << std::endl;
	std::cout << "\t8: Toggle building lots" << std::endl;
	std::cout << "\t9: Toggle buildings" << std::endl;
	std::cout << "\t0: Toggle minor roads" << std::endl;
	std::cout << "\tz: Toggle minor road voronoi points" << std::endl;
	std::cout << std::endl;
	std::cout << "\t WASD - Move camera" << std::endl;
	std::cout << "\t Q/E - (Un)zoom camera" << std::endl;

	while (window.isOpen())
	{
		window.setView(view);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) { window.close(); }

			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) { drawPopMap = !drawPopMap; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) { drawHeightMap = !drawHeightMap; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) { drawWaterBoundaryMap = !drawWaterBoundaryMap; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) { drawQuadTree = !drawQuadTree; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5)) { drawMstNodes = !drawMstNodes; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6)) { drawRoads = !drawRoads; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7)) { drawMST = !drawMST; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8)) { drawBuildingLots = !drawBuildingLots; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9)) { drawBuildings = !drawBuildings; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0)) { drawMinorRoads = !drawMinorRoads; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) { drawVoronoiPointsForMinorRoads = !drawVoronoiPointsForMinorRoads; }

				// Camera movement
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
					view.move(0, -10);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
					view.move(-10, 0);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
					view.move(0, 10);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
					view.move(10, 0);

				// Camera zooming
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
				{
					view.zoom(1.1f);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
				{
					view.zoom(0.9);
				}
			}
		}

		window.clear();

		/* Population map */
		if (drawPopMap)
		{
			int numOfPoints = winSize*winSize;
			sf::VertexArray popPoints(sf::Points, numOfPoints);
			int counter = 0;
			int x = 0, y = 0;
			for (auto vec : popMap)
			{
				for (auto val : vec)
				{
					// Position
					popPoints[counter].position = sf::Vector2f(x, y);

					// Color
					sf::Color pointColour = sf::Color::White;
					float total = highestVal;
					float current = val;

					// How much percentage of the highest value is the current value
					float percentage = (current * 100) / total;

					// What is the alpha value of a single percent
					float singlePercentColour = 255 / 100;

					// How much must we therefore fill in this pixel for
					float fillAmount = singlePercentColour * percentage;

					pointColour.a = fillAmount;
					popPoints[counter].color = pointColour;

					counter++;
					x++;
				}
				y++;
				x = 0;
			}
			window.draw(popPoints);
		}
		
		/* Terrain (Heightmap) */
		if (drawHeightMap)
		{
			int numOfVerts = terrain.Points().size() * terrain.Points()[0].size();
			sf::VertexArray vertPoints(sf::Points, numOfVerts);
			int counter = 0;
			for (auto vec : terrain.Points())
			{
				for (auto v : vec)
				{
					// Position
					vertPoints[counter].position = sf::Vector2f(v->x, v->y);

					// Color
					sf::Color shapeColor = sf::Color::White;
					int total = terrain.Highest();
					float current = v->z;
					current += (abs(terrain.Lowest()));
					float percentage = (current * 100) / total;
					float singlePercent = 255 / 100;
					float fillAmount = singlePercent * percentage;
					shapeColor.a = fillAmount;
					vertPoints[counter].color = shapeColor;

					counter++;
				}
			}
			window.draw(vertPoints);
		}

		/* Water boundary map */
		if (drawWaterBoundaryMap)
		{
			int numOfPoints = winSize*winSize;
			sf::VertexArray waterPoints(sf::Points, numOfPoints);
			int counter = 0;
			for (int y = 0; y < winSize; y++)
			{
				for (int x = 0; x < winSize; x++)
				{
					waterPoints[counter].position = sf::Vector2f(x, y);
					waterPoints[counter].color = (waterData.IsWater(x, y)) ? sf::Color::White : sf::Color(0, 0, 0, 0);
					counter++;
				}
			}

			window.draw(waterPoints);

			//sf::VertexArray verts;
			//for (int y = 0; y < winSize; y++)
			//{
			//	for (int x = 0; x < winSize; x++)
			//	{
			//		if (waterData.IsWater(x, y))
			//		{
			//			verts.append(sf::Vertex(sf::Vector2f(x, y), sf::Color::White));
			//		}
			//		else
			//		{
			//			verts.append(sf::Vertex(sf::Vector2f(x, y), sf::Color::Black));
			//		}
			//	}
			//}
			//window.draw(verts);

			// window.draw(waterMapSprite);
		}

		/* Quad tree */
		if (drawQuadTree)
		{
			for (auto quad : qt->GetHead()->GetTreeChildren())
			{
				sf::Color colour;
				switch (quad->population)
				{
				case QuadPopulation::high:
					colour = sf::Color::Blue;
					break;
				case QuadPopulation::med:
					colour = sf::Color::Red;
					break;
				case QuadPopulation::low:
					colour = sf::Color::Yellow;
					break;
				}

				sf::Vertex vertices[5] =
				{
					sf::Vertex(sf::Vector2f(quad->xOrigin, quad->yOrigin), colour),
					sf::Vertex(sf::Vector2f(quad->xOrigin, quad->yOrigin + quad->height), colour),
					sf::Vertex(sf::Vector2f(quad->xOrigin + quad->width, quad->yOrigin + quad->height), colour),
					sf::Vertex(sf::Vector2f(quad->xOrigin + quad->width, quad->yOrigin), colour),
					sf::Vertex(sf::Vector2f(quad->xOrigin, quad->yOrigin), colour)
				};

				window.draw(vertices, 5, sf::LineStrip);
			}
		}

		/* Major roads */
		if (drawRoads)
		{
			for (Road* road : majorRoads)
			{
				// Draw each segment
				for (Road* segment : road->segments)
				{
					sf::Vertex vertices[2] =
					{
						sf::Vertex(sf::Vector2f(segment->start->x, segment->start->y), sf::Color(255, 0, 255)),
						sf::Vertex(sf::Vector2f(segment->end->x, segment->end->y), sf::Color(255, 0, 255))
					};
					window.draw(vertices, 2, sf::LinesStrip);
				}

				// Draw the road
				//sf::VertexArray roadVertices(sf::LineStrip, road->nodes.size());
				//int nodeCounter = 0;
				//for (RoadNode* node : road->nodes)
				//{
				//	roadVertices[nodeCounter].position = sf::Vector2f(node->position->x, node->position->y);
				//	roadVertices[nodeCounter].color = sf::Color(255, 0, 255);
				//	nodeCounter++;
				//}
				//window.draw(roadVertices);

				// Draw building lots
				if (road->lots.size() > 0)
				{
					if (drawBuildingLots)
					{
						for (BuildingLot* lot : road->lots)
						{
							sf::Vertex vertices[5] =
							{
								sf::Vertex(sf::Vector2f(lot->bottomLeft->x, lot->bottomLeft->y), sf::Color::White),
								sf::Vertex(sf::Vector2f(lot->topLeft->x, lot->topLeft->y), sf::Color::White),
								sf::Vertex(sf::Vector2f(lot->topRight->x, lot->topRight->y), sf::Color::White),
								sf::Vertex(sf::Vector2f(lot->bottomRight->x, lot->bottomRight->y), sf::Color::White),
								sf::Vertex(sf::Vector2f(lot->bottomLeft->x, lot->bottomLeft->y), sf::Color::White)
							};

							window.draw(vertices, 5, sf::LineStrip);
						}
					}
				}
			}
		}

		/* Minor roads */
		if (drawMinorRoads)
		{
			for (Road* road : minorRoads)
			{
				// Draw the road
				sf::VertexArray roadVertices(sf::LineStrip, 2);

				roadVertices[0].position = sf::Vector2f(road->start->x, road->start->y);
				roadVertices[0].color = sf::Color(255, 0, 0, 100);

				roadVertices[1].position = sf::Vector2f(road->end->x, road->end->y);
				roadVertices[1].color = sf::Color(255, 0, 0, 100);

				window.draw(roadVertices);

				if (road->lots.size() > 0)
				{
					if (drawBuildingLots)
					{
						for (BuildingLot* lot : road->lots)
						{
							sf::Vertex vertices[5] =
							{
								sf::Vertex(sf::Vector2f(lot->bottomLeft->x, lot->bottomLeft->y), sf::Color::White),
								sf::Vertex(sf::Vector2f(lot->topLeft->x, lot->topLeft->y), sf::Color::White),
								sf::Vertex(sf::Vector2f(lot->topRight->x, lot->topRight->y), sf::Color::White),
								sf::Vertex(sf::Vector2f(lot->bottomRight->x, lot->bottomRight->y), sf::Color::White),
								sf::Vertex(sf::Vector2f(lot->bottomLeft->x, lot->bottomLeft->y), sf::Color::White)
							};

							window.draw(vertices, 5, sf::LineStrip);
						}
					}
				}
			}
		}

		/* Minor road expansion points */
		if (drawVoronoiPointsForMinorRoads)
		{
			for (auto point : voronoiPoints)
			{
				sf::CircleShape shape;
				shape.setPosition(point.x(), point.y());
				shape.setFillColor(sf::Color::Cyan);
				shape.setRadius(1);
				window.draw(shape);
			}

			//for (V2* point : expandPoints)
			//{
			//	sf::CircleShape shape;
			//	shape.setPosition(point->x, point->y);
			//	shape.setFillColor(sf::Color::Cyan);
			//	shape.setRadius(2);
			//	window.draw(shape);
			//}
		}

		/* MST */
		if (drawMST)
		{
			for (Edge edge : mst.GetTreeEdges())
			{
				sf::Vertex roadVerts[2] =
				{
					sf::Vertex(sf::Vector2f(edge.start->position->x, edge.start->position->y), sf::Color::Blue),
					sf::Vertex(sf::Vector2f(edge.end->position->x, edge.end->position->y), sf::Color::Blue)
				};

				window.draw(roadVerts, 2, sf::Lines);
			}
		}

		/* Points */
		if (drawMstNodes)
		{
			for (auto node : mst.GetNodes())
			{
				sf::CircleShape shape;
				shape.setPosition(node->position->x, node->position->y);
				shape.setFillColor(sf::Color::Green);
				shape.setRadius(1);

				window.draw(shape);
			}
		}

		/* Floor plans, buildings */
		if (drawBuildings)
		{
			for (Road* road : majorRoads)
			{
				for (BuildingLot* lot : road->lots)
				{
					// lot->fp->bb->DrawSelf(&window);
					lot->fp->DrawPerimeter(&window);
				}
			}
		}

		window.display();
	}

	return 0;
}