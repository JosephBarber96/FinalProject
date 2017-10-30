#include "Road.h"
#include "Vec2.h"

Road::Road() {}

Road::Road(Vec2* newStart)
{
	points.push_back(newStart);
}

Road::~Road() {}

void Road::AddPoint(Vec2* newPoint)
{
	points.push_back(newPoint);
}