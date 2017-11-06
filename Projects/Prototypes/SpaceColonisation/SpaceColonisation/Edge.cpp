#include "Edge.h"


Edge::Edge() {}

Edge::Edge(Node* newParent, Node* newChild)
	:
	parent(newParent),
	child(newChild)
{}

Edge::~Edge() {}