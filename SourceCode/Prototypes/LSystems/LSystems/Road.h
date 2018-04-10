#pragma once
#include <vector>
class Vec2;
class Road
{
public:
	Road();
	Road(Vec2* newStart);
	~Road();

	static int roadID;

	bool roadPruned = false;
	bool markedForDeletion = false;
	int id;
	float TotalLength();

	std::vector<Vec2*> points;
	void AddPoint(Vec2* newPoint);

	bool operator== (Road* road);
	bool operator== (Road road);
};

