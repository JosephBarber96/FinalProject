#define _USE_MATH_DEFINES
#include <math.h>

#include "Road.h"
#include "Utility.h"
#include "Vec2.h"
#include "MinorRoad.h"

Road::Road() {}

Road::Road(Vec2* newStart, Vec2* newEnd)
	:
	start(newStart),
	end(newEnd)
{
}

Road::Road(float startX, float startY, float endX, float endY)
	:
	start(new Vec2(startX, startY)),
	end(new Vec2(endX, endY))
{
}

Road::~Road() {}


void Road::CreateMinorRoads(float distanceBetweenRoads, float minorAngle, std::vector<MinorRoad> &minorRoads, std::vector<Road> &majorRoads)
{
	// Get the length of the road
	float length = Utility::DistanceBetween(start, end);

	// How far have we traveled so far
	int distCounter = 0;

	// The road spawning position - it starts on our starting position
	Vec2* roadSpawningPoint = new Vec2(start->getX(), start->getY());

	// Our normalized direction
	Vec2* dir = new Vec2(end->getX() - start->getX(), end->getY() - start->getY());
	Vec2* dirNormalized = dir->Normalized();

	// our facing angle
	angle = atan2f(dir->getY(), dir->getX()) * 180 / M_PI;

	// Loop
	for (;;)
	{
		/*
			Sometimes dir can be equal to (0, 0) which causes dirNormalized
			to be a nullptr
		*/
		if (roadSpawningPoint == nullptr || dirNormalized == nullptr)
		{
			return;
		}

		// Create a minor road
		MinorRoad minorRoad = MinorRoad(roadSpawningPoint->getX(), roadSpawningPoint->getY(), /*minorAngle*/ angle-90, this);
		minorRoad.Extend(majorRoads);
		minorRoads.push_back(minorRoad);

		// Move along the road
		roadSpawningPoint = new Vec2(roadSpawningPoint->getX() + (dirNormalized->getX() * distanceBetweenRoads),
			roadSpawningPoint->getY() + (dirNormalized->getY() * distanceBetweenRoads));

		// How far have we traveled along this major road
		distCounter += distanceBetweenRoads;

		// If we have traveled the length of the road, return
		if (distCounter > length) { return; }

		// If we're too close tot he end of the road, return
		// if (Utility::DistanceBetween(roadSpawningPoint, end) < distanceBetweenRoads) { return; }
	}
}

bool Road::operator==(Road b)
{
	return
		(this->start == b.start
			&&
			this->end == b.end);
}