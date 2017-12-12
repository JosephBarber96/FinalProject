#pragma once
#include "Node.h"
class V2;
class RoadNode : public Node
{
public:
	RoadNode();
	RoadNode(V2* vec);
	RoadNode(int x, int y);
	RoadNode(int x, int y, float cost);
	~RoadNode();

	bool checked;
	float distance;
	RoadNode* parent;
	std::vector<RoadNode*> GetNeighbors(std::vector<std::vector<RoadNode*>> grid);
	std::vector<RoadNode*> neighbours;

	void FillNeighbours(std::vector<std::vector<RoadNode*>> grid, int gridHeight, int gridWidth);
	void SetCost(int newCost);
	float GetCost() { return cost; }

private:
	float cost;
};