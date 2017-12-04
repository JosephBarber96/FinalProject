#include <algorithm>

#include "Road.h"
#include "Vec2.h"
#include "Utility.h"

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