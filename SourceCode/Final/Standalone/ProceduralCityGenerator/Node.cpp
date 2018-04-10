#include "Node.h"
#include "Vec2.h"

Node::Node() {}

Node::Node(int ix, int iy)
	:
	position(new Vec2(ix, iy))
{}


Node::Node(float fx, float fy)
	:
	position(new Vec2(fx, fy))
{}

Node::Node(Vec2* pos)
	:
	position(pos)
{}

Node::~Node() {}

bool Node::operator== (Node rhs)
{
	return (position->x == rhs.position->x && position->y == rhs.position->y);
}