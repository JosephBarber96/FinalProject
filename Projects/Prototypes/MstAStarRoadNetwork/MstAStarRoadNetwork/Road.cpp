#include "Road.h"

std::vector<Road*> Road::roads;

Road::Road()
{
	roads.push_back(this);
}

Road::~Road() {}