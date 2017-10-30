#pragma once
#include <vector>
class Vec2;
class Road
{
public:
	Road();
	Road(Vec2* newStart);
	~Road();

	bool roadPruned = false;

	std::vector<Vec2*> points;
	void AddPoint(Vec2* newPoint);
};

