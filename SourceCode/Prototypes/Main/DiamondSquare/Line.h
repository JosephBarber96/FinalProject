#pragma once
class V2;
class Line
{
public:
	Line();
	Line(V2* s, V2* e);
	Line(float sx, float sy, float ex, float ey);
	~Line();

	V2* start;
	V2* end;
};