#include <math.h>

#include "Vec2.h"
#include "Utility.h"

Vec2::Vec2(float newX, float newY)
	:
	x(newX),
	y(newY)
{}

Vec2::~Vec2() {}

float Vec2::Length()
{
	return sqrt(x * x + y * y);
}
Vec2* Vec2::operator* (float scale)
{
	return new Vec2(x * scale, y * scale);
}

Vec2* Vec2::operator+ (Vec2 vec)
{
	return new Vec2(x + vec.x, y + vec.y);
}

Vec2* Vec2::operator/ (float scale)
{
	return new Vec2(x / scale, y / scale);
}