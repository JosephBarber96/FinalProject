#pragma once
class V2;
class Line
{
public:
	Line();
	Line(V2* _start, V2* _end);
	Line(float sX, float sY, float eX, float ey);
	~Line();

	V2* start, *end;
};

