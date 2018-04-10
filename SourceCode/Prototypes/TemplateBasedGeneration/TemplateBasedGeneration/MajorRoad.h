#pragma once
#include "Road.h"
class Vec2;
class MajorRoad : public Road
{
public:
	MajorRoad();
	MajorRoad(Vec2* newStart, Vec2* newEnd);
	MajorRoad(float startX, float startY, float endX, float endY);
	~MajorRoad();

	void CreateMinorRoads(float distBetweenRoads, float Angle);

	static std::vector<MajorRoad*> getRoads() { return roads; }

private:
	static std::vector<MajorRoad*> roads;
};