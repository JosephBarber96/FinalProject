#pragma once
#include <vector>

#include "Node.h"

class MstNode : public Node
{
public:
	MstNode();
	MstNode(int ix, int iy);
	MstNode(float fx, float fy);
	MstNode(Vec2* pos);
	~MstNode();

	int id;
	bool partOfTree;
	bool checkedForNeighbours;

	void AddPossibleNeighbour(MstNode* neighbour);
	std::vector<MstNode*> GetPossibleNeighbours() const;

private:
	std::vector<MstNode*> possibleNeighbours;
};