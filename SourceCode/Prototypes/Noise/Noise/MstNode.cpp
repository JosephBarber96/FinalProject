#include "MstNode.h"
#include "V2.h"

MstNode::MstNode() {}

MstNode::~MstNode() {}

MstNode::MstNode(V2* vec)
	:
	Node(new V2(vec->x, vec->y))
{}

MstNode::MstNode(int x, int y)
	:
	Node(x, y)
{}

void MstNode::AddPossibleNeighbour(MstNode* neighbour)
{
	possibleNeighbours.push_back(neighbour);
}
