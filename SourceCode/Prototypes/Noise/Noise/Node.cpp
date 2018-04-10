#include "Node.h"
#include "V2.h"

Node::Node() {}

Node::Node(V2* vec)
{
	position = new V2(vec->x, vec->y);
}

Node::Node(int x, int y)
{
	position = new V2(x, y);
}

Node::~Node() {}

bool Node::operator== (Node node)
{
	return (position == node.position);
}