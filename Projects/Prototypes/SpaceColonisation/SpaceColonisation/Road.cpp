#include "Road.h"
#include "Node.h"

std::vector<Road*> Road::roads;

Road::Road() {}

Road::Road(Node* par, Node* chi)
	:
	parent(par),
	child(chi)
{
	roads.push_back(this);
}

Road::~Road() {}