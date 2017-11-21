#include <algorithm>
#include <iostream>
#include <vector>

#include "Vec2.h"
#include "Utility.h"
#include "Road.h"
#include "MinorRoad.h"
#include "MajorRoad.h"

extern std::vector<Vec2*> intersectionPoints;

std::vector<MinorRoad*> MinorRoad::minorRoads;

MinorRoad::MinorRoad(Vec2* newStart, float newAngle, Road* newParent, bool addToMinorRoads)
	:
	Road(newStart, nullptr)
{
	parent = newParent;
	angle = newAngle;
	directionNormalized = Vec2::AngleToVector(angle);

	if (addToMinorRoads)
	{
		minorRoads.push_back(this);
	}
}

MinorRoad::MinorRoad(float startX, float startY, float newAngle, Road* newParent, bool addToMinorRoads)
	:
	Road(new Vec2(startX, startY), nullptr)
{
	parent = newParent;
	angle = newAngle;
	directionNormalized = Vec2::AngleToVector(angle);

	if (addToMinorRoads)
	{
		minorRoads.push_back(this);
	}
}

MinorRoad::~MinorRoad() {}

void MinorRoad::ExtendUntilHit()
{
	// How far to extend
	int extendAmount = 1;

	// Set ourselves an end so it knows our direction
	end = new Vec2(start->getX() + (directionNormalized->getX() * extendAmount), start->getY() + (directionNormalized->getY() * extendAmount));

	// Set end as the intersection point
	end = GetIntersectionPointWithMajorRoad();
}

void MinorRoad::BranchExtend()
{
	// How far to extend
	int extendAmount = 10;

	// Set ourselves an end so it knows our direction
	end = new Vec2(start->getX() + (directionNormalized->getX() * extendAmount), start->getY() + (directionNormalized->getY() * extendAmount));

	// Set end as the intersection point
	end = GetIntersectionPointWithMinorRoad();
}

void MinorRoad::Branch(int distBetweenBranches)
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
		// How far we have traveled along this major road
		distCounter += distBetweenBranches;

		// Move along the road
		roadSpawningPoint = new Vec2(roadSpawningPoint->getX() + (dirNormalized->getX() * distBetweenBranches),
			roadSpawningPoint->getY() + (dirNormalized->getY() * distBetweenBranches));

		// If we've traveled the length of the road, return
		if (distCounter > length) { return; }

		// Create a new minor road
		MinorRoad* newRoad = new MinorRoad(roadSpawningPoint, angle+90, this, false);
		newRoad->BranchExtend();
		branches.push_back(newRoad);
	}
}



bool MinorRoad::CollisionWithMajorRoad()
{
	// Find any intersection
	for (auto road : MajorRoad::getRoads())
	{
		if (*road == *parent) continue;

		Vec2* ip = Utility::GetIntersectionPoint(start, end, road->start, road->end);

		if (ip != nullptr)
		{
			return true;
		}
	}
	return false;
}

bool MinorRoad::CollisionWithMinorRoad()
{
	// Find any intersection
	for (auto road : MinorRoad::getMinorRoads())
	{
		if (*road == *parent) continue;

		Vec2* ip = Utility::GetIntersectionPoint(start, end, road->start, road->end);

		if (ip != nullptr)
		{
			return true;
		}
	}
}

Vec2* MinorRoad::GetIntersectionPointWithMajorRoad()
{
	// To hold all of the points of intersection
	std::vector<Vec2*> iPoints;

	// Find all of the intersections
	for (auto road : MajorRoad::getRoads())
	{
		if (*road == *parent) continue;

		Vec2* ip = Utility::GetIntersectionPoint(start, end, road->start, road->end);

		if (ip != nullptr)
		{
			iPoints.push_back(ip);
		}
	}

	// Return null if we don't find any points
	if (iPoints.size() == 0) { return nullptr; }

	// Otherwise, return the correct intersection 
	return GetCorrectIntersectionPoint(iPoints);
}

Vec2* MinorRoad::GetIntersectionPointWithMinorRoad()
{
	// To hold all of the points of intersection
	std::vector<Vec2*> iPoints;

	// Find all of the intersections
	for (auto road : MinorRoad::getMinorRoads())
	{
		if (*road == *parent) continue;

		Vec2* ip = Utility::GetIntersectionPoint(start, end, road->start, road->end);

		if (ip != nullptr)
		{
			iPoints.push_back(ip);
		}
	}

	for (auto road : MajorRoad::getRoads())
	{
		if (*road == *parent) continue;

		Vec2* ip = Utility::GetIntersectionPoint(start, end, road->start, road->end);

		if (ip != nullptr)
		{
			iPoints.push_back(ip);
		}
	}

	// Return null if we don't find any points
	if (iPoints.size() == 0) { return nullptr; }

	// Otherwise, return the correct intersection
	return GetCorrectIntersectionPoint(iPoints);
}

Vec2* MinorRoad::GetCorrectIntersectionPoint(std::vector<Vec2*> iPoints)
{
	// Local function for use within the lambda
	Vec2* beginning = start;

	// Lambda
	auto closestTo = [beginning](Vec2* a, Vec2* b) -> bool
	{
		return
			(
				Utility::DistanceBetween(a, beginning)
				<
				Utility::DistanceBetween(b, beginning)
				);
	};

	// Sort
	std::sort(iPoints.begin(), iPoints.end(), closestTo);

	int index = 0;
	if (*iPoints[0] == *beginning)
	{
		index = 1;
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

		//// If endX is left of startX but intersectionX is right of startX
		//else if (end->getX() < start->getX() && iPoints[index]->getX() > start->getX())
		//{
		//	index++;
		//}

		//// If endX is right of startX but intersectionX is left of startX
		//else if (end->getX() > start->getY() && iPoints[index]->getX() < start->getX())
		//{
		//	index++;
		//}

		// Else if none of these conditions were hit, return the correct one
		else
		{
			return iPoints[index];
		}

		if (index >= iPoints.size()) { return nullptr; }

	} while (true);
}