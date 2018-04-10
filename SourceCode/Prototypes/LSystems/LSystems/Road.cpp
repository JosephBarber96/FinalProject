#include "Road.h"
#include "Vec2.h"
#include "Utility.h"

int Road::roadID;

Road::Road()
{
	id = roadID++;
}

Road::Road(Vec2* newStart)
{
	points.push_back(newStart);
	id = roadID++;
}

Road::~Road() {}

void Road::AddPoint(Vec2* newPoint)
{
	points.push_back(newPoint);
}

float Road::TotalLength()
{
	if (points.size() < 2) { return 0; }

	float len = 0;

	for (int i = 1; i < points.size()-1; i++)
	{
		len += Utility::DistanceBetween(points[i], points[i - 1]);
	}

	return len;
}

bool Road::operator==(Road* road)
{
	return (id == road->id);
}

bool Road::operator==(Road road)
{
	return (id == road.id);
}