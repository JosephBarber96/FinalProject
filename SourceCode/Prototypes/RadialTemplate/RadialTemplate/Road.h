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

	void CreateMinorRoads(float distanceBetweenRoads, bool left, std::vector<MinorRoad> &minorRoads, std::vector<Road> &colliders);
	void ExtendUntilHit(std::vector<Road> &colliders);
	void CutOffAtIntersection(std::vector<Road> &colliders);
	std::vector<Vec2*> GetAllIntersectionPoints(std::vector<Road> &colliders);

	Vec2* start;
	Vec2* end;

	bool operator==(Road road);

private:
	Vec2* GetIntersectionPoint(std::vector<Road> &colliders);
	Vec2* FindBestIntersectionPoint(std::vector<Vec2*> &iPoints);
};