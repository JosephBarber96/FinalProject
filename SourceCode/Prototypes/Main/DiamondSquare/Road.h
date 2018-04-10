#pragma once
#include <vector>
class RoadNode;
class BuildingLot;
class V2;
class Road
{
public:
	Road();
	Road(int _sx, int _sy, int _ex, int _ey);
	Road(V2* s, V2* e);
	~Road();

	void GenerateBuildingLots();
	void GenerateBuildingLotsForLongRoad();

	bool operator==(Road rhs);

	bool markedForDeletion = false;
	bool expandFromEnd = false;
	bool expandFromStart = false;
	V2* start;
	V2* end;
	std::vector<RoadNode*> nodes;
	std::vector<Road*> segments;
	std::vector<BuildingLot*> lots;
	std::vector<Road*> endChildren;
	std::vector<Road*> startChildren;
};