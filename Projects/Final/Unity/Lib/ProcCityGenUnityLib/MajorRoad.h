#pragma once
#include <vector>

#include "Road.h"

class RoadNode;
class BuildingLot;
class MajorRoad : public Road
{
public:
	MajorRoad();
	MajorRoad(int _sx, int _sy, int _ex, int _ey);
	MajorRoad(Vec2* s, Vec2* e);
	~MajorRoad();

	bool expandFromEnd = false;
	bool expendFromStart = false;

	std::vector<RoadNode*> nodes;
	std::vector<MajorRoad*> segments;
};