#pragma once
#include <vector>

class Vec2;
class Circle
{
public:
	Circle();
	~Circle();
	
	void AddPoint(float x, float y);
	void AddPoint(Vec2* a);

	std::vector<Vec2*> points;
};