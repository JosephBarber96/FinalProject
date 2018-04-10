#include <algorithm>

#include "MinimumSpanningTree.h"
#include "WaterData.h"
#include "MstNode.h"
#include "MstEdge.h"
#include "Vec2.h"
#include "UtilRandom.h"

MinimumSpanningTree::MinimumSpanningTree() {}

MinimumSpanningTree::~MinimumSpanningTree() {}

void MinimumSpanningTree::SpawnPoint(WaterData* wd, int offset, int minX, int minY, int maxX, int maxY)
{
	// Find all valid land positions in this quad
	std::vector<Vec2> landPositions;
	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			if (!wd->IsWater(x, y))
			{
				landPositions.push_back(Vec2(x, y));
			}
		}
	}

	// If none exist, return without placing a point
	if (landPositions.size() == 0) { return; }

	// Choose a random land position
	int index = UtilRandom::Instance()->RandomInt(0, landPositions.size() - 1);

	int x = landPositions[index].x;
	int y = landPositions[index].y;

	// Snap it to our offset
	int xdifference = (x % offset);
	x = x - xdifference;

	int ydifference = (y % offset);
	y = y - ydifference;

	// Add the node
	MstNode* node = new MstNode(x, y);
	node->id = nodeCount++;
	AddNode(node);
}

void MinimumSpanningTree::AssignNeighbours(float maxDistance)
{
	// For each node
	for (MstNode* node : nodes)
	{
		bool neighbourFound = false;
		MstNode* closestNeighbour = nullptr;

		// For every other ndoe
		for (MstNode* possibleNeighbour : nodes)
		{
			// Skip itself
			if (*node == *possibleNeighbour) { continue; }

			// The first neighbour is the closest if closestNeighbour is nullptr
			if (closestNeighbour == nullptr)
			{
				closestNeighbour = possibleNeighbour;
				continue;
			}

			// If possible neighbour is closer than closest neighbour, replace it
			if (Vec2::DistanceBetween(*node->position, *possibleNeighbour->position) < Vec2::DistanceBetween(*node->position, *closestNeighbour->position))
			{
				closestNeighbour = possibleNeighbour;
			}

			// If possibleNeighbour is close enough, add it as a possible neighbour
			if (Vec2::DistanceBetween(*node->position, *possibleNeighbour->position) < maxDistance)
			{
				neighbourFound = true;
				node->AddPossibleNeighbour(possibleNeighbour);
			}
		}

		// After checking each neighbour, if a close enough neighbour wasnt found, add the closest neighbour
		if (!neighbourFound)
		{
			node->AddPossibleNeighbour(closestNeighbour);
		}
	}
}

void MinimumSpanningTree::CreateEdges()
{
	// For every node
	for (MstNode* node : nodes)
	{
		// For each neighbour
		for (MstNode* neighbour : node->GetPossibleNeighbours())
		{
			if (neighbour->checkedForNeighbours) { continue; }
			AddPossibleEdge(new MstEdge(node, neighbour));
		}
		node->checkedForNeighbours = true;
	}
}

void MinimumSpanningTree::Sort()
{
	// Completely reset the tree
	treeEdges.clear();
	for (MstNode* node : nodes)
	{
		node->partOfTree = false;
	}
	for (MstEdge* edge : allEdges)
	{
		edge->partOfTree = false;
	}

	// Start by sorting all edges
	std::sort(allEdges.begin(), allEdges.end(), [](MstEdge* lhs, MstEdge* rhs) -> bool { return lhs->Length() < rhs->Length(); });

	// Pointer to hold edges
	MstEdge* shortest;

	// To begin the tree we find the shortest which is now the front
	shortest = allEdges.front();
	allEdges.erase(allEdges.begin());

	shortest->partOfTree = true;
	shortest->start->partOfTree = true;
	shortest->end->partOfTree = true;
	treeEdges.push_back(shortest);


	// Loop...
	bool completed = false;

	int iter = 0;
	while (!completed)
	{
		// Reset
		iter = 0;
		bool nodeFound = false;

		// Find the node of lowest cost that is connected to the MST...
		for (auto edge : allEdges)
		{
			iter++;
			// If start and not end
			if (edge->start->partOfTree && !edge->end->partOfTree)
			{
				shortest = new MstEdge(edge->start, edge->end);
				allEdges.erase(allEdges.begin() + iter);
				break;
			}
			// Or end and not start
			else if (edge->end->partOfTree && !edge->start->partOfTree)
			{
				shortest = new MstEdge(edge->start, edge->end);
				allEdges.erase(allEdges.begin() + iter);
				break;
			}
		}

		//Mark them
		shortest->partOfTree = true;
		shortest->start->partOfTree = true;
		shortest->end->partOfTree = true;

		// Add the shortest to the MST...
		treeEdges.push_back(shortest);

		// Check to see if all nodes are now a part of the tree..
		bool allInTree = true;
		for (MstNode* node : nodes)
		{
			if (!node->partOfTree)
			{
				allInTree = false;
				break;
			}
		}

		// If they all are in MST...
		if (allInTree)
		{
			// stop looping
			completed = true;
		}
	}
}

bool MinimumSpanningTree::TreeEdgeContainsNode(MstNode* node)
{
	for (MstEdge* edge : treeEdges)
	{
		if (*edge->start == *node || *edge->end == *node)
		{
			return true;
		}
	}
	return false;
}

void MinimumSpanningTree::AddNode(MstNode* newNode)
{
	nodes.push_back(newNode);
}

void MinimumSpanningTree::AddPossibleEdge(MstEdge* newEdge)
{
	allEdges.push_back(newEdge);
}