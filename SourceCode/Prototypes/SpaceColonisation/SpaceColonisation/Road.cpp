#include "Road.h"
#include "Node.h"

std::vector<Road*> Road::roads;

Road::Road() {}

Road::Road(Node* start)
{
	AddPoint(start);
}

Road::~Road() {}

void Road::AddPoint(Node* point)
{
	points.push_back(point);
}