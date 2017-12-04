#include "Circle.h"
#include "Vec2.h"

Circle::Circle() {}

Circle::~Circle() {}

void Circle::AddPoint(float x, float y)
{
	points.push_back(new Vec2(x, y));
}

void Circle::AddPoint(Vec2* a)
{
	points.push_back(a);
}