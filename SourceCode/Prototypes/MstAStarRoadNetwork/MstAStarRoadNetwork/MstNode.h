#pragma once
#include "Node.h"
class V2;
class MstNode : public Node
{
public:
	MstNode();
	MstNode(V2* vec);
	MstNode(int x, int y);
	~MstNode();

	int id;
	bool partOfTree = false;
	bool checkedForNeighbours = false;

	std::vector<MstNode*> getPossibleNeighbours() { return possibleNeighbours; }

	void AddPossibleNeighbour(MstNode* neighbour);

private:
	std::vector<MstNode*> possibleNeighbours;
};

