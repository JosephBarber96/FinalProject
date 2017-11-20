#include "Road.h"
#include "Utility.h"
#include "Vec2.h"
#include "MinorRoad.h"

std::vector<Road*> Road::roads;

Road::Road() {}

Road::Road(Vec2* newStart, Vec2* newEnd)
	:
	start(newStart),
	end(newEnd)
{
	roads.push_back(this);
}

Road::Road(float startX, float startY, float endX, float endY)
	:
	start(new Vec2(startX, startY)),
	end(new Vec2(endX, endY))
{
	roads.push_back(this);
}

Road::~Road() {}

void Road::CreateMinorRoads(float distBetweenRoads, float angle)
{
	float length = Utility::DistanceBetween(start, end);
	
	int distCounter = 0;

	Vec2* roadSpawningPoint = start;

	Vec2* dir = new Vec2(end->getX() - start->getX(), end->getY() - start->getY());
	Vec2* dirNormalized = dir->Normalized();
	
	while (true)
	{
		// How far have we traveled along this major road
		distCounter += distBetweenRoads;

		// If we've traveled the length of it, return
		if (distCounter > length) { return; }

		// Create a new minor road
		MinorRoad* newRoad = new MinorRoad(roadSpawningPoint, angle, this);

		// Move along the road
		roadSpawningPoint = new Vec2(roadSpawningPoint->getX() + (dirNormalized->getX() * distBetweenRoads),
			roadSpawningPoint->getY() + (dirNormalized->getY() * distBetweenRoads));
	}
}

bool Road::operator==(Road b)
{
	return
		(this->start == b.start
			&&
			this->end == b.end);
}