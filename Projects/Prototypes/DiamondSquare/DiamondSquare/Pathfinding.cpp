#include <algorithm>
#include <iostream>

#include "Pathfinding.h"
#include "RoadNode.h"
#include "MstNode.h"
#include "V2.h"

namespace Pathfinding
{
	std::vector<RoadNode*> PathFind(std::vector<std::vector<RoadNode*>> grid, int startX, int startY, int endX, int endY, int offsetPerRoadNode)
	{
		// Start, End
		RoadNode* start = grid[startY][startX];
		RoadNode* end = grid[endY][endX];

		if (start == nullptr || end == nullptr)
		{
			return std::vector<RoadNode*>();
		}

		// Q : the set of all nodes
		std::vector<RoadNode*> Q;
		for (auto vec : grid)
		{
			for (auto node : vec)
			{
				node->distance = FLT_MAX;
				node->parent = nullptr;
				Q.push_back(node);
			}
		}

		// The start has a distance of 0
		start->distance = 0;
		start->checked = true;

		while (!Q.empty())
		{
			//u is node in Q with the smallest distance...

			// Sort it
			std::sort(Q.begin(), Q.end(), [end](const RoadNode* lhs, const RoadNode *rhs) -> bool
			{
				return (lhs->distance + V2::Length(*lhs->position - *end->position)
					<
					(rhs->distance + V2::Length(*rhs->position - *end->position)));
			});

			// Pick the front
			RoadNode* u = Q.front();

			// Remove u from Q
			Q.erase(std::remove(Q.begin(), Q.end(), u), Q.end());

			// For each neighbour of u
			for (RoadNode* neighbour : u->GetNeighbors(grid))
			{
				//if (neighbour->checked) continue;
				// Skip non-walkables
				// if (!neighbour->isWalkable) continue;

				//neighbour->checked = true;

				auto alt = neighbour->GetCost() + V2::Length(*neighbour->position - *end->position);

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
		int counter = 0;
		do
		{
			road.push_back(goal);
			goal = goal->parent;
		} while (goal->parent != nullptr);
		road.push_back(goal);

		return road;
	}
}

