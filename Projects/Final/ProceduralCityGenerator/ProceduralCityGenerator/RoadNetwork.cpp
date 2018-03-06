#include <iostream>

#include <boost\polygon\voronoi.hpp>

#include "Utility.h"
#include "UtilRandom.h"
#include "Pathfinding.h"
#include "Vec2.h"

#include "RoadNetwork.h"
#include "MajorRoad.h"
#include "MinorRoad.h"
#include "RoadNode.h"
#include "MstEdge.h"
#include "MstNode.h"
#include "MinimumSpanningTree.h"
#include "WaterData.h"
#include "BuildingLot.h"
#include "Line.h"

using namespace boost::polygon;
typedef int coordinate_type;
typedef point_data<coordinate_type> point_type;
typedef voronoi_diagram<double> VD;

RoadNetwork::RoadNetwork()
{
	minorRoads = std::vector<MinorRoad*>();
	majorRoads = std::vector<MajorRoad*>();
	roadNodes = std::vector<std::vector<RoadNode*>>();
}

RoadNetwork::~RoadNetwork() {}

void RoadNetwork::GenerateMinorRoads(VD* voronoi)
{
	for (auto const &edge : voronoi->edges())
	{
		if (edge.vertex0() != NULL && edge.vertex1() != NULL)
		{
			int sX = edge.vertex0()->x();
			int sY = edge.vertex0()->y();

			int eX = edge.vertex1()->x();
			int eY = edge.vertex1()->y();

			// See if this exists already
			bool exists = false;
			for (auto r : minorRoads)
			{
				// If this is the same road but reversed (start->end vs end->start)
				if (r->End()->x == sX && r->End()->y == sY
					&& r->Start()->x == eX && r->Start()->y == eY)
				{
					exists = true;
					break;
				}
			}
			if (exists) continue;

			// If not, create
			MinorRoad* road = new MinorRoad(sX, sY, eX, eY);
			minorRoads.push_back(road);
		}
	}
}

void RoadNetwork::GenerateMajorRoads(MinimumSpanningTree* mst, int offsetForRoadNodes)
{
	std::cout << "Total edges: " << mst->GetAllEdges().size() << std::endl;
	std::cout << "Pathfinding " << mst->GetTreeEdges().size() << " roads..." << std::endl;
	int roadCounter = 0;

	for (MstEdge* edge : mst->GetTreeEdges())
	{
		if (edge->start->position->x == edge->end->position->x && edge->start->position->y == edge->end->position->y)
		{
			continue;
		}

		// Create the road
		MajorRoad* road = new MajorRoad();

		// Pathfind to get the nodes
		road->nodes = Pathfinding::PathFind(roadNodes,
			edge->start->position->x / offsetForRoadNodes,
			edge->start->position->y / offsetForRoadNodes,
			edge->end->position->x / offsetForRoadNodes,
			edge->end->position->y / offsetForRoadNodes,
			offsetForRoadNodes);

		// Split the road into segments
		for (std::vector<RoadNode*>::iterator iter = road->nodes.begin(); iter != road->nodes.end(); iter++)
		{
			if (Utility::ReturnNextIter(iter) != road->nodes.end())
			{
				road->segments.push_back(
					new MajorRoad((*iter)->position, (*Utility::ReturnNextIter(iter))->position));
			}
		}

		majorRoads.push_back(road);

		std::cout << "\r" << roadCounter++ << " roads complete.\t\t";
	}
	std::cout << std::endl;
}

void RoadNetwork::ValidateRoads(WaterData* waterData)
{
	/*******************************************************
			Remove any voronoi minor roads that
			overlap main mst roads and reconnect the
			voronoi to the main roads
	********************************************************/
	std::vector<Vec2*> expandPoints;

	std::cout << "Checking for minor roads to prune..." << std::endl;

	// For each minor road
	for (MinorRoad* minor : minorRoads)
	{
		// Check each major road
		for (MajorRoad* major : majorRoads)
		{
			// Check each segment
			for (auto seg : major->segments)
			{
				if (Utility::GetIntersectionPointWithFiniteLines(minor->Start(), minor->End(), seg->Start(), seg->End()) != nullptr)
				{
					// Mark this road for deletion
					minor->markedForDeletion = true;

					// Tell its neighbours to expand and find a major road to connect to
					expandPoints.push_back(new Vec2(minor->Start()->x, minor->Start()->y));
					expandPoints.push_back(new Vec2(minor->End()->x, minor->End()->y));
				}
			}
		}
	}

	std::cout << "Deleting roads." << std::endl;

	// Remove and minor roads that have been marked for deletion
	for (std::vector<MinorRoad*>::iterator iter = minorRoads.begin(); iter != minorRoads.end(); /**/)
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

	std::cout << "Reconnecting minor roads to highways" << std::endl;

	// For each point
	for (Vec2* point : expandPoints)
	{
		// Find the closest roadNode to it
		RoadNode* closest = nullptr;

		// Check each node
		for (auto road : majorRoads)
		{
			for (RoadNode* node : road->nodes)
			{
				// The first will be closest by default
				if (closest == nullptr) { closest = node; }

				// If node is closer than closest, closest becomes node
				if (Vec2::DistanceBetween(*node->position, *point) < Vec2::DistanceBetween(*closest->position, *point))
				{
					closest = node;
				}
			}
		}

		// After finding the closest, create this new road
		minorRoads.push_back(
			new MinorRoad(point->x,
				point->y,
				closest->position->x,
				closest->position->y));
	}

	// Now that we've built new minor roads, we need to check for collisions between any minor roads

	std::cout << "Checking for new collisions." << std::endl;

	for (auto minorRoad : minorRoads)
	{
		for (auto other : minorRoads)
		{
			// Skip itself
			if (*minorRoad == *other) { continue; }

			// if Other is already marked for deletion, skip
			// We don't want both roads to get deleted which would cause there to be 
			// 0 connections instead of 2 (we want there to be 1)
			if (other->markedForDeletion) { continue; }

			// Get the intersection point
			Vec2* iPoint = Utility::GetIntersectionPointWithFiniteLines(minorRoad->Start(), minorRoad->End(), other->Start(), other->End());

			// If it's not null
			if (iPoint != nullptr)
			{
				// Make sure its not where the roads naturally connect
				if (*iPoint == *minorRoad->Start()
					|| *iPoint == *minorRoad->End()
					|| *iPoint == *other->Start()
					|| *iPoint == *other->End())
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
	for (auto minorRoad : minorRoads)
	{
		if (waterData->IsWater(minorRoad->Start()->x, minorRoad->Start()->y)
			||
			waterData->IsWater(minorRoad->End()->x, minorRoad->End()->y))
		{
			minorRoad->markedForDeletion = true;
		}
	}

	// Next we delete any minor roads that are above a certain length threshold
	// this is to remove the long outwards lines left over from the voronoi
	for (auto minorRoad : minorRoads)
	{
		if (Vec2::DistanceBetween(*minorRoad->Start(), *minorRoad->End()) > maxMinorRoadDist)
		{
			minorRoad->markedForDeletion = true;
		}
	}

	// Remove any new minor roads that have been marked for deletion
	for (std::vector<MinorRoad*>::iterator iter = minorRoads.begin(); iter != minorRoads.end(); /**/)
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

	std::cout << "Neighbouring minor roads " << std::endl;

	// Now that we've deleted all of these roads, neighbour the remaining roads
	for (auto minorRoad : minorRoads)
	{
		// Against every other road
		for (auto otherRoad : minorRoads)
		{
			// Skip itself
			if (*minorRoad == *otherRoad) { continue; }

			// Check for neighbours

			// If otherRoad has minorRoad.Start
			if (*minorRoad->Start() == *otherRoad->Start()
				||
				*minorRoad->Start() == *otherRoad->End())
			{
				minorRoad->startChildren.push_back(minorRoad);
			}

			// If otherRoad has minorRoad.End
			if (*minorRoad->End() == *otherRoad->Start()
				||
				*minorRoad->End() == *otherRoad->End())
			{
				minorRoad->endChildren.push_back(minorRoad);
			}
		}
	}

	std::cout << "Removing minor roads with zero neighbours" << std::endl;

	// Remove any minor roads that have 0 neighbours (and thus are sitting along)
	for (std::vector<MinorRoad*>::iterator iter = minorRoads.begin(); iter != minorRoads.end(); /**/)
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
}

void RoadNetwork::GenerateBuildingLots()
{
	std::cout << "Generating building lots on minor roads" << std::endl;
	int lotCounter = 0;
	for (MinorRoad* road : minorRoads)
	{
		std::cout << "\rGenerating lot of road: " << lotCounter++ << "\t\t";
		road->GenerateBuildingLots();
	}
	std::cout << std::endl;
}

void RoadNetwork::ValidateBuildingLots()
{
	std::cout << "Checking for building lot collisions..." << std::endl;

	/* MINOR ROADS */

	/* First, we prioritive collisions against minor roads and major roads */
	std::cout << "Between minor roads..." << std::endl;
	// For every building lot
	for (MinorRoad* minorRoad : minorRoads)
	{
		for (BuildingLot* lot : minorRoad->lots)
		{
			// Check every minor road
			for (MinorRoad* otherRoad : minorRoads)
			{
				// Skip itself
				if (*minorRoad == *otherRoad) { continue; }

				// Check if each line of the lot intersects with the otherRoad
				for (Line* line : lot->lines)
				{
					if (Utility::GetIntersectionPointWithFiniteLines(line->start, line->end, otherRoad->Start(), otherRoad->End()) != nullptr)
					{
						lot->markForDeletion = true;
					}
				}
			}
		}
	}

	std::cout << "Between major roads..." << std::endl;
	int majorRoadsChecked = 0;
	// For every building lot
	for (MinorRoad* minorRoad : minorRoads)
	{
		for (BuildingLot* lot : minorRoad->lots)
		{
			// For every major road
			for (MajorRoad* majorRoad : majorRoads)
			{
				// Check each segment of the major road
				for (auto seg : majorRoad->segments)
				{
					// For each line of the buildnig lot
					for (auto line : lot->lines)
					{
						if (Utility::GetIntersectionPointWithFiniteLines(line->start, line->end, seg->Start(), seg->End()) != nullptr)
						{
							lot->markForDeletion = true;
						}
					}
				}
			}
		}
	}

	int minorLotDeletionCounter = 0;
	for (MinorRoad* road : minorRoads)
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

	std::cout << "Between other building lots..." << std::endl;
	// Check every road against every other road
	for (MinorRoad* minorRoad : minorRoads)
	{
		for (MinorRoad* otherRoad : minorRoads)
		{
			// Do not check the road against itself
			if (*minorRoad == *otherRoad) { continue; }

			// Check each lot of each road
			for (BuildingLot* lot : minorRoad->lots)
			{
				for (BuildingLot* otherLot : otherRoad->lots)
				{
					// If the lots are far away we do not need to check for interceptions
					if (Vec2::DistanceBetween(*lot->bottomLeft, *otherLot->bottomLeft) > 15)
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

	for (MinorRoad* road : minorRoads)
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

	std::cout << minorLotDeletionCounter << " lots pruned." << std::endl;
}

void RoadNetwork::GenerateBuildings()
{

}