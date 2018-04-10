#include "MajorRoad.h"
#include "MinorRoad.h"
#include "Utility.h"
#include "Vec2.h"

std::vector<MajorRoad*> MajorRoad::roads;

MajorRoad::MajorRoad() {}

MajorRoad::MajorRoad(Vec2* newStart, Vec2* newEnd)
	:
	Road(newStart, newEnd)
{
	roads.push_back(this);
}

MajorRoad::MajorRoad(float startX, float startY, float endX, float endY)
	:
	Road(startX, startY, endX, endY)
{
	roads.push_back(this);
}

MajorRoad::~MajorRoad() {}

void MajorRoad::CreateMinorRoads(float distBetweenRoads, float angle)
{
	// Get the length of the road
	float length = Utility::DistanceBetween(start, end);

	// How far we have traveled so far
	int distCounter = 0;

	// Our current position
	Vec2* roadSpawningPoint = start;

	// Our normalized direction
	Vec2* dir = new Vec2(end->getX() - start->getX(), end->getY() - start->getY());
	Vec2* dirNormalized = dir->Normalized();

	// Loop
	while (true)
	{
		// How far have we traveled along this major road
		distCounter += distBetweenRoads;

		// Move along the road
		roadSpawningPoint = new Vec2(roadSpawningPoint->getX() + (dirNormalized->getX() * distBetweenRoads),
			roadSpawningPoint->getY() + (dirNormalized->getY() * distBetweenRoads));

		// If we've traveled the length of it, return
		if (distCounter > length) { return; }

		// If we're too close to the end of the road, return
		if (Utility::DistanceBetween(roadSpawningPoint, end) < distBetweenRoads) { return; }

		// Create a new minor road
		MinorRoad* newRoad = new MinorRoad(roadSpawningPoint, angle, this, true);
		// Extend it
		newRoad->ExtendUntilHit();


	}
}

