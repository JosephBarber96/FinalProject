#pragma once

class Vec2;
class Road
{
public:
	Road();
	Road(int _sx, int _sy, int _ex, int _ey);
	Road(Vec2* s, Vec2* e);
	~Road();

	bool Equals(int sx, int sy, int ex, int ey);

private:
	Vec2* startPos;
	Vec2* endPos;
};