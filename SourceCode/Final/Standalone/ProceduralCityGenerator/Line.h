#pragma once
class Vec2;
class Line
{
public:
	Line();
	Line(Vec2* s, Vec2* e);
	Line(float sx, float sy, float ex, float ey);
	~Line();

	Vec2* start;
	Vec2* end;
};