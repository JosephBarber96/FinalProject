#include "Node.h"

std::vector<Node*> Node::nodes;

Node::Node(int newX, int newY)
	:
	x(newX),
	y(newY)
{
	nodes.push_back(this);
}

Node::~Node() {}