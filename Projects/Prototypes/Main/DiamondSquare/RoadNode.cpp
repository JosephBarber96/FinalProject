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
	if (neighbours.size() == 0)
	{
		FillNeighbours(grid, grid[0].size() - 1, grid.size());
	}
	return neighbours;
}

void RoadNode::FillNeighbours(std::vector<std::vector<RoadNode*>> grid, int gridWidth, int gridHeight)
{
	// Up
	if (yIndex > 0) { neighbours.push_back(grid[yIndex - 1][xIndex]); }

	// Up-right
	if (yIndex > 0 && xIndex < gridWidth-1) { neighbours.push_back(grid[yIndex - 1][xIndex + 1]); }

	// Right
	if (xIndex < gridWidth) { neighbours.push_back(grid[yIndex][xIndex + 1]); }

	// Down-right
	if (yIndex < gridHeight-1 && xIndex < gridWidth-1) { neighbours.push_back(grid[yIndex+1][xIndex+1]); }

	// Down
	if (yIndex < gridHeight-1) { neighbours.push_back(grid[yIndex+1][xIndex]); }

	// Down-left
	if (yIndex < gridHeight-1 && xIndex > 0) { neighbours.push_back(grid[yIndex+1][xIndex-1]); }

	// Left
	if (xIndex > 0) { neighbours.push_back(grid[yIndex][xIndex-1]); }

	// Up-left
	if (yIndex > 0 && xIndex > 0) { neighbours.push_back(grid[yIndex-1][xIndex-1]); }
}

void RoadNode::SetCost(int newCost)
{
	cost = newCost;
}