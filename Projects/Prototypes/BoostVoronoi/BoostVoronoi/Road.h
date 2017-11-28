#pragma once
#include <vector>
class Vec2;
class MinorRoad;
class Road
{
public:
	Road();
	Road(Vec2* newStart, Vec2* newEnd);
	Road(float startX, float startY, float endX, float endY);
	~Road();

	void CreateMinorRoads(float distanceBetweenRoads, float Angle, std::vector<MinorRoad> &minorRoads, std::vector<Road> &majorRoads);

	float angle;
	Vec2* start;
	Vec2* end;
	Vec2* directionNormalized;

	bool operator==(Road road);
};

