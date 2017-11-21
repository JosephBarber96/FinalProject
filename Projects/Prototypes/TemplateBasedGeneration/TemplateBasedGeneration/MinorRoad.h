#pragma once
#include <vector>
#include "Road.h"
class MajorRoad;
class Vec2;
class MinorRoad : public Road
{
public:
	MinorRoad(Vec2* newStart, float newAngle, Road* parent, bool addToMinorRoads);
	MinorRoad(float startX, float startY, float newAngle, Road* newParent, bool addToMinorRoads);
	~MinorRoad();

	Road* parent;

	void ExtendUntilHit();
	void Branch(int distBetweenBranches);
	void BranchExtend();
	bool CollisionWithMajorRoad();
	bool CollisionWithMinorRoad();
	Vec2* GetIntersectionPointWithMajorRoad();
	Vec2* GetIntersectionPointWithMinorRoad();
	std::vector<MinorRoad*> getBranches() const { return branches; }

	static std::vector<MinorRoad*> getMinorRoads() { return minorRoads; }

private:
	std::vector<MinorRoad*> branches;
	Vec2* GetCorrectIntersectionPoint(std::vector<Vec2*> iPoints);
	static std::vector<MinorRoad*> minorRoads;
};

