#include "Road.h"
#include "BuildingLot.h"
#include "RoadNode.h"
#include "V2.h"

Road::Road() {}

Road::Road(int _sx, int _sy, int _ex, int _ey)
	:
	start(new V2(_sx, _sy)),
	end(new V2(_ex, _ey))
{}


Road::Road(V2* s, V2* e)
	:
	start(s),
	end(e)
{}

Road::~Road() {}

void Road::GenerateBuildingLots()
{
	//// Only plot lots on straight roads (only 2 nodes - start and end)
	//if (nodes.size() > 2) { return; }

	//V2* start = nodes[0]->position;
	//V2* end = nodes[1]->position;

	float roadLength = V2::DistanceBetween(*start, *end);

	V2* direction = new V2(end->x - start->x, end->y - start->y);
	V2* dirNormalized = direction->Normalized();
	if (dirNormalized == nullptr) return;

	float minSize = 4.f;
	float gap = 2.f;
	float lotLength = roadLength;

	while (lotLength > minSize)
	{
		lotLength *= 0.5f;
	}

	bool quit = false;
	V2* lotPos = new V2(start->x, start->y);

	// Before we begin, travel down the size of the lot and the buffer space
	lotPos->x += dirNormalized->x * minSize;
	lotPos->y += dirNormalized->y * minSize;
	lotPos->x += dirNormalized->x * gap;
	lotPos->y += dirNormalized->y * gap;

	while (!quit)
	{
		// Travel down the road the size of the lot
		lotPos->x += dirNormalized->x * minSize;
		lotPos->y += dirNormalized->y * minSize;

		// Place lot left
		lots.push_back(new BuildingLot(new V2(lotPos->x, lotPos->y), minSize, this, 0));
		// place lot right
		lots.push_back(new BuildingLot(new V2(lotPos->x, lotPos->y), minSize, this, 1));

		// Travel down the road buffer space between lots
		lotPos->x += dirNormalized->x * gap;
		lotPos->y += dirNormalized->y * gap;

		if (V2::DistanceBetween(*lotPos, *end) < minSize * 2.5f + 10)
		{
			quit = true;
		}
	}
}

void Road::GenerateBuildingLotsForLongRoad()
{
	return;

	int size = nodes.size();
	int minSize = 2;

	while (size > minSize * 2)
	{
		size *= 0.5f;
	}

	int originalSize = size;
	while (size + originalSize * 2 < nodes.size())
	{
		size += originalSize;

		lots.push_back(new BuildingLot(nodes[size]->position, minSize, this, 0));
	}
}

bool Road::operator==(Road rhs)
{
	return (*start == *rhs.start && *end == *rhs.end);
}