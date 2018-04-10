#include "MstNode.h"

MstNode::MstNode() {}

MstNode::MstNode(int ix, int iy)
	:
	Node(ix, iy)
{}


MstNode::MstNode(float fx, float fy)
	:
	Node(fx, fy)
{}

MstNode::MstNode(Vec2* pos)
	:
	Node(pos)
{}

MstNode::~MstNode() {}

void MstNode::AddPossibleNeighbour(MstNode* neighbour)
{
	possibleNeighbours.push_back(neighbour);
}

std::vector<MstNode*> MstNode::GetPossibleNeighbours() const
{
	return possibleNeighbours;
}