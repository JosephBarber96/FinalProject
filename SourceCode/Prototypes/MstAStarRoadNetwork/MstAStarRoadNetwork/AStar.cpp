#include <algorithm>

#include "AStar.h"
#include "RoadNode.h"
#include "MstNode.h"
#include "Utility.h"
#include "V2.h"

namespace AStar
{
	std::vector<RoadNode*> PathFind(int startX, int startY, int endX, int endY)
	{
		// Start, End
		RoadNode* start = RoadNode::GetNodeAtPosition(startX, startY);
		RoadNode* end = RoadNode::GetNodeAtPosition(endX, endY);

		if (start == nullptr || end == nullptr)
		{
			return std::vector<RoadNode*>();
		}

		//RoadNode* start = RoadNode::grid[startY][startX];
		//RoadNode* end = RoadNode::grid[endY][endX];

		// Q : the set of all nodes
		std::vector<RoadNode*> Q;
		for (auto vec : RoadNode::grid)
		{
			for (auto node : vec)
			{
				node->distance = FLT_MAX;
				node->parent = nullptr;
				Q.push_back(node);
			}
		}

		//for (int y = 0; y < RoadNode::grid.size()-1; y++)
		//{
		//	for (int x = 0; x < RoadNode::grid[0].size()-1; x++)
		//	{
		//		RoadNode::grid[y][x]->distance = FLT_MAX;
		//		RoadNode::grid[y][x]->parent = nullptr;
		//		Q.push_back(RoadNode::grid[y][x]);
		//	}
		//}

		// The start has a distance of 0
		start->distance = 0;

		while (!Q.empty())
		{

			//u is node in Q with the smallest distance
			RoadNode* u = Q.front();
			for (RoadNode* rn : Q)
			{
				if (
					(rn->distance + Utility::Length(*rn->position - *end->position))
					<
					(u->distance + Utility::Length(*rn->position - *end->position))
					)
				{
					u = rn;
				}
			}

			// Remove u from Q
			Q.erase(std::remove(Q.begin(), Q.end(), u), Q.end());

			// For each neighbour of u
			for (RoadNode* neighbour : u->neighbours)
			{
				// Skip non-walkables
				// if (!neighbour->isWalkable) continue;

				auto alt = neighbour->GetCost() + Utility::Length(*neighbour->position - *end->position);

				if (alt < neighbour->distance)
				{
					neighbour->distance = alt;
					neighbour->parent = u;
					if (neighbour->position->x == end->position->x
						&&
						neighbour->position->y == end->position->y)
					{
						return ReconstructPath(neighbour);
					}
				}
			}
		}
	}

	std::vector<RoadNode*> ReconstructPath(RoadNode* goal)
	{
		std::vector<RoadNode*> road = std::vector<RoadNode*>();
		do
		{
			road.push_back(goal);
			goal = goal->parent;
		} while (goal->parent != nullptr);
		road.push_back(goal);

		return road;
	}
}

