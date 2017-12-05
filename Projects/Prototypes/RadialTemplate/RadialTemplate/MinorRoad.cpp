#include <algorithm>

#include "MinorRoad.h"
#include "Vec2.h"
#include "Utility.h"

MinorRoad::MinorRoad() {}

MinorRoad::MinorRoad(Vec2* newStart, float newAngle, Road* newParent)
	:
	Road(newStart, nullptr)
{
	Init(newAngle, newParent);
}

MinorRoad::MinorRoad(float startX, float startY, float newAngle, Road* newParent)
	:
	Road(new Vec2(startX, startY), nullptr)
{
	Init(newAngle, newParent);
}

MinorRoad::~MinorRoad() {}

void MinorRoad::Extend(std::vector<Road> &colliders)
{
	// How far to extend;
	int extendAmount = 1000;

	// Set ourselves an end so it knows our direction
	end = new Vec2(start->getX() + (directionNormalized->getX() * extendAmount),
		start->getY() + (directionNormalized->getY() * extendAmount));

	// Get the intersection point
	Vec2* ip = GetIntersectionPoint(colliders);

	if (ip != nullptr)
	{
		end = ip;
	}
	else
	{
		end = start;
	}
}

Vec2* MinorRoad::GetIntersectionPoint(std::vector<Road> &colliders)
{
	// To hold all of the points of intersection
	std::vector <Vec2*> iPoints;

	// Find all of the intersections
	for (auto road : colliders)
	{
		// Skip our parent
		if (parent != nullptr)
		{
			if (road == *parent) { continue; }
		}

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
	return GetBestIntersectionPoint(iPoints);
}

Vec2* MinorRoad::GetBestIntersectionPoint(std::vector<Vec2*> &iPoints)
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

void MinorRoad::Init(float newAngle, Road* newParent)
{
	parent = newParent;
	angle = newAngle;
	directionNormalized = Vec2::AngleToVector(angle);
}