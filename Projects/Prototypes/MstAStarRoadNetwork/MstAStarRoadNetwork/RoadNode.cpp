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