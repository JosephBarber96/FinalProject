#include "RoadNode.h"
#include "V2.h"

RoadNode::RoadNode() {}

RoadNode::RoadNode(V2* vec)
	:
	Node(new V2(vec->x, vec->y))
{}

RoadNode::RoadNode(int x, int y)
	:
	Node(x, y)
{}

RoadNode::RoadNode(int x, int y, float _cost)
	:
	Node(x, y),
	cost(_cost)
{}

RoadNode::~RoadNode() {}

std::vector<RoadNode*> RoadNode::GetNeighbors(std::vector<std::vector<RoadNode*>> grid)
{
	std::vector<RoadNode*> myNeighbours;

	// Up
	if (position->y > 0) { myNeighbours.push_back(grid[position->y - 1][position->x]); }

	// Up-right
	if (position->y > 0 && position->x < grid[0].size()-1) { myNeighbours.push_back(grid[position->y - 1][position->x + 1]); }

	// Right
	if (position->x < grid[0].size()-1) { myNeighbours.push_back(grid[position->y][position->x + 1]); }

	// Down-right
	if (position->y < grid.size()-1 && position->x < grid[0].size()-1) { myNeighbours.push_back(grid[position->y + 1][position->x + 1]); }

	// Down
	if (position->y < grid.size()-1) { myNeighbours.push_back(grid[position->y + 1][position->x]); }

	// Down-left
	if (position->y < grid.size()-1 && position->x > 0) { myNeighbours.push_back(grid[position->y + 1][position->x - 1]); }

	// Left
	if (position->x > 0) { myNeighbours.push_back(grid[position->y][position->x - 1]); }

	// Up-left
	if (position->y > 0 && position->x > 0) { myNeighbours.push_back(grid[position->y - 1][position->x - 1]); }

	return myNeighbours;
}

void RoadNode::FillNeighbours(std::vector<std::vector<RoadNode*>> grid, int gridHeight, int gridWidth)
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