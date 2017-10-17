#include "RoadNode.h"
#include "V2.h"

std::vector<std::vector<RoadNode*>> RoadNode::grid;

RoadNode::RoadNode() {}

RoadNode::RoadNode(V2* vec)
	:
	Node(new V2(vec->x, vec->y))
{}

RoadNode::RoadNode(int x, int y)
	:
	Node(x, y)
{}

RoadNode::~RoadNode() {}

void RoadNode::FillNeighbours()
{
	// Up
	if (yIndex > 0) { neighbours.push_back(grid[yIndex - 1][xIndex]); }

	// Up-right
	if (yIndex > 0 && xIndex < grid[yIndex].size() - 1) { neighbours.push_back(grid[yIndex - 1][xIndex + 1]); }

	// Right
	if (xIndex < grid[yIndex].size() - 1) { neighbours.push_back(grid[yIndex][xIndex + 1]); }

	// Down-right
	if (yIndex < grid.size() - 1 && xIndex < grid[yIndex].size() - 1) { neighbours.push_back(grid[yIndex + 1][xIndex + 1]); }

	// Down
	if (yIndex < grid.size() - 1) { neighbours.push_back(grid[yIndex + 1][xIndex]); }

	// Down-left
	if (yIndex > grid.size() - 1 && xIndex > 0) { neighbours.push_back(grid[yIndex + 1][xIndex - 1]); }

	// Left
	if (xIndex > 0) { neighbours.push_back(grid[yIndex][xIndex - 1]); }

	// Up-left
	if (yIndex > 0 && xIndex > 0) { neighbours.push_back(grid[yIndex - 1][xIndex - 1]); }
}

void RoadNode::FillNeighbours(int gridHeight, int gridWidth)
{
	// Up
	if (position->y > 0) { neighbours.push_back(grid[position->y - 1][position->x]); }

	// Up-right
	if (position->y > 0 && position->x < gridWidth) { neighbours.push_back(grid[position->y - 1][position->x + 1]); }

	// Right
	if (position->x < gridWidth) { neighbours.push_back(grid[position->y][position->x + 1]); }

	// Down-right
	if (position->y < gridHeight && position->x < gridWidth) { neighbours.push_back(grid[position->y + 1][position->x + 1]); }

	// Down
	if (position->y < gridHeight) { neighbours.push_back(grid[position->y + 1][position->x]); }

	// Down-left
	if (position->y < gridHeight && position->x > 0) { neighbours.push_back(grid[position->y + 1][position->x - 1]); }

	// Left
	if (position->x > 0) { neighbours.push_back(grid[position->y][position->x - 1]); }

	// Up-left
	if (position->y > 0 && position->x > 0) { neighbours.push_back(grid[position->y - 1][position->x - 1]); }
}

void RoadNode::SetCost(int newCost)
{
	cost = newCost;
}

RoadNode* RoadNode::GetNodeAtPosition(int xPos, int yPos)
{
	for (auto vec : grid)
	{
		for (auto node : vec)
		{
			if (node->position->x == xPos
				&&
				node->position->y == yPos)
			{
				return node;
			}
		}
	}

	return nullptr;
}