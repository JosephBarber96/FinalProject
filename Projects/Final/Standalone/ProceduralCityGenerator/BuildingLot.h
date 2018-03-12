#pragma once
#include <vector>

class Vec2;
class Road;
class Line;
class FloorPlan;
class BuildingLot
{
public:
	BuildingLot();
	BuildingLot(Vec2* botLeft, float size, Road* par, int _dir);
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
	Vec2* bottomLeft;
	Vec2* topLeft;
	Vec2* topRight;
	Vec2* bottomRight;
	Road* parent;

	int minX, maxX, minY, maxY;

	std::vector<Line*> lines;

	FloorPlan* fp;

	bool operator== (BuildingLot rhs);

private:
	void GetOutwardValues();
};