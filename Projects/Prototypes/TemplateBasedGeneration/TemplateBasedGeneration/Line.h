#pragma once
class Vec2;
class Line
{
public:
	Line(Vec2* newStart, Vec2* newEnd);
	Line(float startX, float startY, float endX, float endY);
	~Line();

	Vec2* start;
	Vec2* end;

	bool operator== (Line b);
};

