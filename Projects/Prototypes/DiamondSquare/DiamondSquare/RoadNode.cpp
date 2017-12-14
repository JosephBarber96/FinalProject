#include "RoadNode.h"
#include "V2.h"

RoadNode::RoadNode() {}

RoadNode::RoadNode(int _xIndex, int _yIndex, V2* vec)
	:
	Node(new V2(vec->x, vec->y)),
	xIndex(_xIndex),
	yIndex(_yIndex)
{}

RoadNode::RoadNode(int _xIndex, int _yIndex, int x, int y)
	:
	Node(x, y),
	xIndex(_xIndex),
	yIndex(_yIndex)
{}

RoadNode::RoadNode(int _xIndex, int _yIndex, int x, int y, float _cost)
	:
	Node(x, y),
	xIndex(_xIndex),
	yIndex(_yIndex),
	cost(_cost)
{}

RoadNode::~RoadNode() {}

std::vector<RoadNode*> RoadNode::GetNeighbors(std::vector<std::vector<RoadNode*>> grid)
{
	std::vector<RoadNode*> myNeighbours;

	// Up
	if (yIndex > 0) { myNeighbours.push_back(grid[yIndex - 1][xIndex]); }

	// Up-right
	if (yIndex > 0 && xIndex < grid[0].size()-1) { myNeighbours.push_back(grid[yIndex-1][xIndex+1]); }

	// Right
	if (xIndex < grid[0].size()-1) { myNeighbours.push_back(grid[yIndex][xIndex+1]); }

	// Down-right
	if (yIndex < grid.size()-1 && xIndex < grid[0].size()-1) { myNeighbours.push_back(grid[yIndex+1][xIndex+1]); }

	// Down
	if (yIndex < grid.size()-1) { myNeighbours.push_back(grid[yIndex+1][xIndex]); }

	// Down-left
	if (yIndex < grid.size()-1 && xIndex > 0) { myNeighbours.push_back(grid[yIndex+1][xIndex-1]); }

	// Left
	if (xIndex > 0) { myNeighbours.push_back(grid[yIndex][xIndex-1]); }

	// Up-left
	if (yIndex > 0 && xIndex > 0) { myNeighbours.push_back(grid[yIndex-1][xIndex-1]); }

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