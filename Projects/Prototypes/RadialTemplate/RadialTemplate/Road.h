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

	void ExtendUntilHit(std::vector<Road> &colliders);
	std::vector<Vec2*> GetAllIntersectionPoints(std::vector<Road> &colliders);

	Vec2* start;
	Vec2* end;

private:
	Vec2* GetIntersectionPoint(std::vector<Road> &colliders);
	Vec2* FindBestIntersectionPoint(std::vector<Vec2*> &iPoints);
};