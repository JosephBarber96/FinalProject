#pragma once
#include "Road.h"
class Vec2;
class MinorRoad : public Road
{
public:
	MinorRoad();
	MinorRoad(Vec2* newStart, float newAngle, Road* newParent);
	MinorRoad(float startX, float startY, float newAngle, Road* newParent);
	~MinorRoad();

	float angle;
	Road* parent;
	Vec2* directionNormalized;

	void Extend(std::vector<Road> &colliders);

private:
	void Init(float newAngle, Road* newParent);
	Vec2* GetIntersectionPoint(std::vector<Road> &colliders);
	Vec2* GetBestIntersectionPoint(std::vector<Vec2*> &iPoints);
};

