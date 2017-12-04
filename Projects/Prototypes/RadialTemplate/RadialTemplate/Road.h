#pragma once
class Vec2;
class Road
{
public:
	Road();
	Road(Vec2* newStart, Vec2* newEnd);
	Road(float startX, float startY, float endX, float endY);
	~Road();

	Vec2* start;
	Vec2* end;
};