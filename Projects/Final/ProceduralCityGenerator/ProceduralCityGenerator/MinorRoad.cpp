#include "MinorRoad.h"
#include "Vec2.h"
#include "BuildingLot.h"

MinorRoad::MinorRoad() {}

MinorRoad::MinorRoad(int _sx, int _sy, int _ex, int _ey)
	:
	Road(_sx, _sy, _ex, _ey)
{}

MinorRoad::MinorRoad(Vec2* s, Vec2* e)
	:
	Road(s, e)
{}

MinorRoad::~MinorRoad() {}

void MinorRoad::GenerateBuildingLots()
{
	//// Only plot lots on straight roads (only 2 nodes - start and end)
	//if (nodes.size() > 2) { return; }

	//V2* start = nodes[0]->position;
	//V2* end = nodes[1]->position;

	float roadLength = Vec2::DistanceBetween(*startPos, *endPos);

	Vec2* direction = new Vec2(endPos->x - startPos->x, endPos->y - startPos->y);
	Vec2* dirNormalized = direction->Normalized();
	if (dirNormalized == nullptr) return;

	float minSize = 4.f;
	float gap = 2.f;
	float lotLength = roadLength;
	float minSpaceRequired = minSize + gap + 1;

	while (lotLength > minSize)
	{
		lotLength *= 0.5f;
	}

	bool quit = false;
	Vec2* lotPos = new Vec2(startPos->x, startPos->y);

	// Before we begin, travel down the size of the lot and the buffer space
	lotPos->x += dirNormalized->x * gap;
	lotPos->y += dirNormalized->y * gap;

	// Check there is enough space to post even 1 lot
	if (Vec2::DistanceBetween(*lotPos, *endPos) < minSpaceRequired)
	{
		return;
	}

	// If there is...
	while (!quit)
	{
		// Place lot left
		lots.push_back(new BuildingLot(new Vec2(lotPos->x, lotPos->y), minSize, this, 0));
		// place lot right
		lots.push_back(new BuildingLot(new Vec2(lotPos->x, lotPos->y), minSize, this, 1));

		// Travel down the road the size of the lot
		lotPos->x += dirNormalized->x * minSize;
		lotPos->y += dirNormalized->y * minSize;

		// Travel down the road buffer space between lots
		lotPos->x += dirNormalized->x * gap;
		lotPos->y += dirNormalized->y * gap;

		// Check if we have enough space to loop again
		if (Vec2::DistanceBetween(*lotPos, *endPos) < minSpaceRequired)
		{
			quit = true;
		}
	}
}

bool MinorRoad::operator==(MinorRoad rhs)
{
	return (*startPos == *rhs.Start() && *endPos == *rhs.End());
}