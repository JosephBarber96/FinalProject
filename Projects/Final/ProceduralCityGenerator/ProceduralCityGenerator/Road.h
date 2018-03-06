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

	Vec2* Start() const { return startPos; }
	Vec2* End() const { return endPos; }

protected:
	Vec2* startPos;
	Vec2* endPos;
};