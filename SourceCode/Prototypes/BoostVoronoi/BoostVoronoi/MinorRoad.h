#pragma once
#include "Road.h"
class MinorRoad : public Road
{
public:
	MinorRoad();
	MinorRoad(Vec2* newStart, float newAngle, Road* newParent);
	MinorRoad(float startX, float startY, float newAngle, Road* newParent);
	~MinorRoad();

	Road* parent;

	void Extend(std::vector<Road> &majorRoads);
	

private:
	void Init(float newAngle, Road* newParent);
	Vec2* GetIntersectionPoint(std::vector<Road> &majorRoad);
	Vec2* GetBestIntersectionPoint(std::vector<Vec2*> &iPoints);
};

