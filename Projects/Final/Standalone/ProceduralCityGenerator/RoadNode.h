#pragma once
#include <vector>

#include "Node.h"

class Vec2;
class RoadNode : public Node
{
public:
	RoadNode();
	RoadNode(int _xIndex, int _yIndex, Vec2* pos);
	RoadNode(int _xIndex, int _yIndex, int x, int y);
	RoadNode(int _xIndex, int _yIndex, int x, int y, float cost);
	~RoadNode();

	bool checked;
	float distance;
	RoadNode* parent;
	std::vector<RoadNode*> GetNeighbours(std::vector<std::vector<RoadNode*>> grid);

	void SetCost(int newCost);
	
	int GetXIndex() const { return xIndex; }
	int GetYIndex() const { return yIndex; }
	float GetCost() const { return cost; }

private:
	void FillNeighbours(std::vector<std::vector<RoadNode*>> grid, int gridWidth, int gridHeight);

	int xIndex, yIndex;
	float cost;
	std::vector<RoadNode*> neighbours;
};

