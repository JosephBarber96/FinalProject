#pragma once
class Vec2;
class Line
{
public:
	Line(Vec2* newStart, Vec2* newEnd);
	~Line();

	Vec2* getStart() { return start; }
	Vec2* getEnd() { return end; }

private:
	Vec2* start;
	Vec2* end;
};

