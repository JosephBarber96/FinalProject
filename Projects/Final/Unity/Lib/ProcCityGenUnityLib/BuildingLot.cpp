#include "BuildingLot.h"
#include "Vec2.h"
#include "Road.h"
#include "RoadNode.h"
#include "Line.h"
#include "Utility.h"

#include <iostream>

BuildingLot::BuildingLot() {}

BuildingLot::BuildingLot(Vec2* botLeft, float size, Road* par, int _dir)
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
	Vec2* parentStart = parent->Start();
	Vec2* parentEnd = parent->End();
	//V2* parentStart = parent->nodes[0]->position;
	//V2* parentEnd = parent->nodes[1]->position;

	// Find the parents facing angle
	Vec2* parentDir = new Vec2(parentEnd->x - parentStart->x, parentEnd->y - parentStart->y);
	float parentAngle = Vec2::VectorToAngle(parentDir);

	// Find our facing angle (relatively outwards from bottomLeft)
	int rotateAmount = (_dir == 0) ? -90 : 90;

	float myAngle = parentAngle + rotateAmount;
	angle = myAngle;
	dir = _dir;

	// Find our direction
	Vec2* myDir = Vec2::AngleToVector(myAngle);

	// Starting position
	Vec2* currentPosition = new Vec2(bottomLeft->x, bottomLeft->y);

	/* bot Left */

	// Move forwards ever so slightly to distance ourselves from the road
	currentPosition->x += myDir->x;
	currentPosition->y += myDir->y;

	// Assign
	bottomLeft = new Vec2(currentPosition->x, currentPosition->y);

	/* top Left */

	// Move up
	currentPosition->x += myDir->x * size;
	currentPosition->y += myDir->y * size;

	// Assign
	topLeft = new Vec2(currentPosition->x, currentPosition->y);

	/* top Right */

	// rotate right 90 degrees
	myAngle += rotateAmount;
	myDir = Vec2::AngleToVector(myAngle);

	// Move up
	currentPosition->x += myDir->x * size;
	currentPosition->y += myDir->y * size;

	// Assign
	topRight = new Vec2(currentPosition->x, currentPosition->y);

	/* bottom Right */

	// rotate right 90 degrees
	myAngle += rotateAmount;
	myDir = Vec2::AngleToVector(myAngle);

	// Move up
	currentPosition->x += myDir->x * size;
	currentPosition->y += myDir->y * size;

	// Assign
	bottomRight = new Vec2(currentPosition->x, currentPosition->y);

	// Generate lines
	GenerateLotLines();

	// Get outward values
	GetOutwardValues();
}

bool BuildingLot::IsLotWithin(BuildingLot* otherLot)
{
	//for (auto line : lines)
	//{
	//	for (auto otherline : otherLot->lines)
	//	{
	//		if (Utility::GetIntersectionPointWithFiniteLines(line->start, line->end, otherline->start, otherline->end) != nullptr)
	//		{
	//			return true;
	//		}
	//	}
	//}
	//return false;

	bool minXWithin = false,
		maxXWithin = false,
		minYWithin = false,
		maxYWithin = false;

	// otherMinX within
	if (otherLot->minX > minX && otherLot->minX < maxX) minXWithin = true;
	// otherMaxX within
	if (otherLot->maxX < maxX && otherLot->maxX > minX) maxXWithin = true;
	// otherMinY within
	if (otherLot->minY > minY && otherLot->minY < maxY) minYWithin = true;
	// otherMaxY within
	if (otherLot->maxY < maxY && otherLot->maxY > minY) maxYWithin = true;

	/* Checks */

	// If minimum X value is within bounds

	if (minXWithin)
	{
		return (minYWithin || maxYWithin);
	}
	else if (maxXWithin)
	{
		return (minYWithin || maxYWithin);
	}
	return false;
}

bool BuildingLot::PointWithin(int x, int y)
{
	return (x >= minX && x <= maxX && y >= minY && y <= maxY);
}

void BuildingLot::GetOutwardValues()
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

void BuildingLot::GenerateLotLines()
{
	lines;

	// botLeft -> topLeft
	lines.push_back(new Line(bottomLeft, topLeft));

	// topLeft -> topRight
	lines.push_back(new Line(topLeft, topRight));

	// topRight -> bottomRight
	lines.push_back(new Line(topRight, bottomRight));

	// bottomRight -> botLeft
	lines.push_back(new Line(bottomRight, bottomLeft));
}

float BuildingLot::GetWidth()
{
	return Vec2::DistanceBetween(*bottomLeft, *bottomRight);
}

float BuildingLot::GetHeight()
{
	return Vec2::DistanceBetween(*bottomLeft, *topLeft);
}

bool BuildingLot::operator==(BuildingLot rhs)
{
	return (*bottomLeft == *rhs.bottomLeft
		&& *topLeft == *rhs.topLeft
		&& *topRight == *rhs.topRight
		&& *bottomRight == *rhs.bottomRight);
}