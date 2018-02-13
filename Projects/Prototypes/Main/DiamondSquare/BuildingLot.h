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
	bool PointWithin(int x, int y);
	
	void GenerateLotLines();
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

	int minX, maxX, minY, maxY;

	std::vector<Line*> lines;

	FloorPlan* fp;

	bool operator== (BuildingLot rhs);

private:
	void GetOutwardValues();
};