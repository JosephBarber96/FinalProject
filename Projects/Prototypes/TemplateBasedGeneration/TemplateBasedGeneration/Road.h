#pragma once
#include <vector>

class Vec2;
class Road
{
public:
	Road();
	Road(Vec2* newStart, Vec2* newEnd);
	Road(float startX, float startY, float endX, float endY);
	~Road();

	Vec2* start;
	Vec2* end;

	void CreateMinorRoads(float distBetweenRoads, float Angle);

	bool operator== (Road b);

	static std::vector<Road*> getRoads() { return roads; }

private:
	static std::vector<Road*> roads;
};
