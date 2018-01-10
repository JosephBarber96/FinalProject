#pragma once
#include <vector>

class V2;
class Road;
class Line;
class BuildingLot
{
public:
	BuildingLot();
	BuildingLot(V2* botLeft, float size, Road* par, int dir);
	~BuildingLot();

	void Expand(float size, Road* parent, int dir);
	bool IsLotWithin(BuildingLot* lot);
	void GetOutwardValues(float &minX, float &maxX, float &minY, float &maxY);
	std::vector<Line*> GetLotLines();

	bool markForDeletion = false;
	V2* bottomLeft;
	V2* topLeft;
	V2* topRight;
	V2* bottomRight;

	bool operator== (BuildingLot rhs);
};

