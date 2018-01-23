#pragma once
#include <vector>

class V2;
class Road;
class Line;
class FloorPlan;
class BuildingLot
{
public:
	BuildingLot();
	BuildingLot(V2* botLeft, float size, Road* par, int _dir);
	~BuildingLot();

	void Expand(float size, Road* parent, int dir);
	bool IsLotWithin(BuildingLot* lot);
	void GetOutwardValues(float &minX, float &maxX, float &minY, float &maxY);
	std::vector<Line*> GetLotLines();
	float GetWidth();
	float GetHeight();

	bool markForDeletion = false;
	float angle;
	int dir;
	V2* bottomLeft;
	V2* topLeft;
	V2* topRight;
	V2* bottomRight;
	Road* parent;

	FloorPlan* fp;

	bool operator== (BuildingLot rhs);
};