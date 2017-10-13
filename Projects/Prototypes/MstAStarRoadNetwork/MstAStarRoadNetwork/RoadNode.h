#pragma once
#include "Node.h"
class V2;
class RoadNode : public Node
{
public:

	static std::vector<std::vector<RoadNode*>> grid;

	RoadNode();
	RoadNode(V2* vec);
	RoadNode(int x, int y);
	~RoadNode();

	bool checked;
	float distance;
	RoadNode* parent;
	std::vector<RoadNode*> neighbours;

	void FillNeighbours(int gridHeight, int gridWidth);
	void SetCost(int newCost);

	inline bool GetCost() { return cost; }

private:
	float cost;
};