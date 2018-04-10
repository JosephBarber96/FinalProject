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

	float angle;
	Vec2* start;
	Vec2* end;
	Vec2* directionNormalized;

	bool operator==(Road road);
};


