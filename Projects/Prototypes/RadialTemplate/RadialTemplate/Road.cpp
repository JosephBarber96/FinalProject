#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#include "Road.h"
#include "Vec2.h"
#include "Utility.h"
#include "MinorRoad.h"

Road::Road() {}

Road::Road(Vec2* newStart, Vec2* newEnd)
	:
	start(newStart),
	end(newEnd)
{}
Road::Road(float startX, float startY, float endX, float endY)
	:
	start(new Vec2(startX, startY)),
	end(new Vec2(endX, endY))
{}

Road::~Road() {}

void Road::CreateMinorRoads(float distanceBetweenRoads, bool left, std::vector<MinorRoad> &minorRoads, std::vector<Road> &colliders)
{
	float angle;

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

		// Do we create the road branching left or right from this major road
		float minorAngle = (left) ? angle - 90 : angle + 90;

		// Create a minor road
		MinorRoad minorRoad = MinorRoad(roadSpawningPoint->getX(), roadSpawningPoint->getY(), /*minorAngleArg*/ minorAngle, this);
		minorRoad.Extend(colliders);
		minorRoads.push_back(minorRoad);

		// Move along the road
		roadSpawningPoint = new Vec2(roadSpawningPoint->getX() + (dirNormalized->getX() * distanceBetweenRoads),
			roadSpawningPoint->getY() + (dirNormalized->getY() * distanceBetweenRoads));

		// How far have we traveled along this major road
		distCounter += distanceBetweenRoads;

		// If we have traveled the length of the road, return
		if (distCounter > length) { return; }

		// If we're too close tot he end of the road, return
		if (Utility::DistanceBetween(roadSpawningPoint, end) < distanceBetweenRoads) { return; }
	}
}

void Road::ExtendUntilHit(std::vector<Road> &colliders)
{
	// Get our direction normalized
	Vec2* dir = new Vec2(end->x - start->x, end->y - start->y);
	dir = dir->Normalized();

	// How much will we scale by to hit the bounding box
	int scaleAmount = 1000;

	// Scale
	end = new Vec2(start->x + (dir->x * scaleAmount), start->y + (dir->y * scaleAmount));

	end = GetIntersectionPoint(colliders);
}

std::vector<Vec2*> Road::GetAllIntersectionPoints(std::vector<Road> &colliders)
{
	// To hold the points of intersection
	std::vector<Vec2*> iPoints;

	// For all roads
	for (auto road : colliders)
	{
		//Vec2* ip = Utility::GetIntersectionPoint(start, end, road.start, road.end);
		Vec2* ip = Utility::GetLineIntersectionPointWithFiniteLine(start, end, road.start, road.end);

		if (ip != nullptr)
		{
			iPoints.push_back(ip);
		}
	}

	return iPoints;
}

Vec2* Road::GetIntersectionPoint(std::vector<Road> &colliders)
{
	// To hold all of the points of intersection
	std::vector <Vec2*> iPoints;

	// Find all of the intersections
	for (auto road : colliders)
	{
		//// Skip our parent
		//if (parent != nullptr)
		//{
		//	if (road == *parent) { continue; }
		//}


		//Vec2* ip = Utility::GetIntersectionPoint(start, end, road.start, road.end);
		Vec2* ip = Utility::GetLineIntersectionPointWithFiniteLine(start, end, road.start, road.end);

		if (ip != nullptr)
		{
			iPoints.push_back(ip);
		}
	}

	// Return null if we don't find any points
	if (iPoints.size() == 0) { return nullptr; }

	// Otherwise, return the correct intersection
	return FindBestIntersectionPoint(iPoints);
}

Vec2* Road::FindBestIntersectionPoint(std::vector<Vec2*> &iPoints)
{
	// Local variable for use within the lambda
	Vec2* beginning = start;

	// Lambda
	auto closestTo = [beginning](Vec2* a, Vec2* b) -> bool { return (Utility::DistanceBetween(a, beginning) < Utility::DistanceBetween(b, beginning)); };

	// Sort
	std::sort(iPoints.begin(), iPoints.end(), closestTo);

	// Skip the beginning
	int index = 0;
	if (*iPoints[0] == *beginning)
	{
		index = 1;

		// If that was the only one, return null
		if (iPoints.size() == 1) { return nullptr; }
	}

	do
	{
		// If endY is below startY but intersectionY is above startY
		if (end->getY() > start->getY() && iPoints[index]->getY() < start->getY())
		{
			// Move to next
			index++;
		}

		// If endY is above startY but intersectionY is below startY
		else if (end->getY() < start->getY() && iPoints[index]->getY() > start->getY())
		{
			index++;
		}

		else
		{
			return iPoints[index];
		}

		if (index >= iPoints.size()) { return nullptr; }

	} while (true);
}

bool Road::operator==(Road b)
{
	return
		(
			this->start == b.start
			&&
			this->end == b.end);
}