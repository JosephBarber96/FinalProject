#include "BuildingLot.h"
#include "V2.h"
#include "Road.h"
#include "RoadNode.h"
#include "Line.h"
#include "Utility.h"

#include <iostream>

BuildingLot::BuildingLot() {}

BuildingLot::BuildingLot(V2* botLeft, float size, Road* par, int _dir)
	:
	bottomLeft(botLeft),
	parent(par)
{
	markForDeletion = false;
	Expand(size, par, _dir);
}

BuildingLot::~BuildingLot() {}

void BuildingLot::Expand(float size, Road* parent, int _dir)
{
	// Get parent start and end
	V2* parentStart = parent->start;
	V2* parentEnd = parent->end;
	//V2* parentStart = parent->nodes[0]->position;
	//V2* parentEnd = parent->nodes[1]->position;

	// Find the parents facing angle
	V2* parentDir = new V2(parentEnd->x - parentStart->x, parentEnd->y - parentStart->y);
	float parentAngle = V2::VectorToAngle(parentDir);

	// Find our facing angle (relatively outwards from bottomLeft)
	int rotateAmount = (_dir == 0) ? -90 : 90;

	float myAngle = parentAngle + rotateAmount;
	angle = myAngle;
	dir = _dir;

	// Find our direction
	V2* myDir = V2::AngleToVector(myAngle);

	// Starting position
	V2* currentPosition = new V2(bottomLeft->x, bottomLeft->y);

	/* bot Left */

	// Move forwards ever so slightly to distance ourselves from the road
	currentPosition->x += myDir->x;
	currentPosition->y += myDir->y;

	// Assign
	bottomLeft = new V2(currentPosition->x, currentPosition->y);

	/* top Left */
	
	// Move up
	currentPosition->x += myDir->x * size;
	currentPosition->y += myDir->y * size;

	// Assign
	topLeft = new V2(currentPosition->x, currentPosition->y);

	/* top Right */

	// rotate right 90 degrees
	myAngle += rotateAmount;
	myDir = V2::AngleToVector(myAngle);

	// Move up
	currentPosition->x += myDir->x * size;
	currentPosition->y += myDir->y * size;

	// Assign
	topRight = new V2(currentPosition->x, currentPosition->y);

	/* bottom Right */

	// rotate right 90 degrees
	myAngle += rotateAmount;
	myDir = V2::AngleToVector(myAngle);

	// Move up
	currentPosition->x += myDir->x * size;
	currentPosition->y += myDir->y * size;

	// Assign
	bottomRight = new V2(currentPosition->x, currentPosition->y);
}

bool BuildingLot::IsLotWithin(BuildingLot* otherLot)
{
	for (auto line : GetLotLines())
	{
		for (auto otherline : otherLot->GetLotLines())
		{
			if (Utility::GetIntersectionPointWithFiniteLines(line->start, line->end, otherline->start, otherline->end) != nullptr)
			{
				return true;
			}
		}
	}
	return false;

	//float minX, maxX, minY, maxY;
	//float otherMinX, otherMaxX, otherMinY, otherMaxY;

	//GetOutwardValues(minX, maxX, minY, maxY);
	//otherLot->GetOutwardValues(otherMinX, otherMaxX, otherMinY, otherMaxY);

	//// Allow for a 5% leeway
	//// minX *= 0.9f; maxX *= 1.1f; minY *= 0.9f; maxY *= 1.1f;

	//bool minXWithin = false,
	//	maxXWithin = false,
	//	minYWithin = false,
	//	maxYWithin = false;

	//// otherMinX within
	//if (otherMinX > minX && otherMinX < maxX) minXWithin = true;
	//// otherMaxX within
	//if (otherMaxX < maxX && otherMaxX > minX) maxXWithin = true;
	//// otherMinY within
	//if (otherMinY > minY && otherMinY < maxY) minYWithin = true;
	//// otherMaxY within
	//if (otherMaxY < maxY && otherMaxY > minY) maxYWithin = true;

	///* Checks */

	//// If minimum X value is within bounds

	//if (minXWithin)
	//{
	//	if (minYWithin || maxYWithin) return true;
	//	return false;
	//}
	//else if (maxXWithin)
	//{
	//	if (minYWithin || maxYWithin) return true;
	//	return false;
	//}
	//else
	//{
	//	return false;
	//}
}

void BuildingLot::GetOutwardValues(float &minX, float &maxX, float &minY, float &maxY)
{
	minX = bottomLeft->x;
	if (topLeft->x < minX) minX = topLeft->x;
	if (topRight->x < minX) minX = topRight->x;
	if (bottomRight->x < minX) minX = bottomRight->x;

	maxX = bottomLeft->x;
	if (topLeft->x > maxX) maxX = topLeft->x;
	if (topRight->x > maxX) maxX = topRight->x;
	if (bottomRight->x > maxX) maxX = bottomRight->x;

	minY = bottomLeft->y;
	if (topLeft->y < minY) minY = topLeft->y;
	if (topRight->y < minY) minY = topRight->y;
	if (bottomRight->y < minY) minY = bottomRight->y;

	maxY = bottomLeft->y;
	if (topLeft->y > maxY) maxY = topLeft->y;
	if (topRight->y > maxY) maxY = topRight->y;
	if (bottomRight->y > maxY) maxY = bottomRight->y;
}

std::vector<Line*> BuildingLot::GetLotLines()
{
	std::vector<Line*> lines;

	// botLeft -> topLeft
	lines.push_back(new Line(bottomLeft, topLeft));

	// topLeft -> topRight
	lines.push_back(new Line(topLeft, topRight));

	// topRight -> bottomRight
	lines.push_back(new Line(topRight, bottomRight));

	// bottomRight -> botLeft
	lines.push_back(new Line(bottomRight, bottomLeft));

	return lines;
}

float BuildingLot::GetWidth()
{
	return V2::DistanceBetween(*bottomLeft, *bottomRight);
}

float BuildingLot::GetHeight()
{
	return V2::DistanceBetween(*bottomLeft, *topLeft);
}

bool BuildingLot::operator==(BuildingLot rhs)
{
	return (*bottomLeft == *rhs.bottomLeft
		&& *topLeft == *rhs.topLeft
		&& *topRight == *rhs.topRight
		&& *bottomRight == *rhs.bottomRight);
}