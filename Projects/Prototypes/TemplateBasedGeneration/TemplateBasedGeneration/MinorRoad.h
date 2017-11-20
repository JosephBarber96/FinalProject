#pragma once
#include <vector>
class Road;
class Vec2;
class MinorRoad
{
public:
	MinorRoad(Vec2* newStart, float angle, Road* parent);
	MinorRoad(float startX, float startY, float angle, Road* newParent);
	~MinorRoad();

	Vec2* start;
	Vec2* end;
	Vec2* dirNormalized;
	Road* parent;

	void ExtendUntilHit();
	bool Collision();
	Vec2* getIntersectionPoint();

	static std::vector<MinorRoad*> getMinorRoads() { return minorRoads; }

private:
	static std::vector<MinorRoad*> minorRoads;
};

