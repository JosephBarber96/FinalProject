#define _USE_MATH_DEFINES
#include <math.h>

#include "V2.h"

V2::V2() {}

V2::V2(float _x, float _y)
	:
	x(_x),
	y(_y)
{}

V2::~V2() {}

float V2::Length()
{
	return sqrt(x * x + y * y);
}

float V2::Length(V2 vec)
{
	return (float)sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

float V2::DistanceBetween(V2 left, V2 right)
{
	V2* vec = new V2(left.x - right.x, left.y - right.y);
	return fabsf(vec->Length());
}

bool V2::operator==(V2 v)
{
	return (x == v.x && y == v.y);
}

bool V2::operator!=(V2 v)
{
	if (x == v.x && y == v.y)
		return false;
	return true;
}