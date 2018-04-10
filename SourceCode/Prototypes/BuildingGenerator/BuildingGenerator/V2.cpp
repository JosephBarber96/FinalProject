#include "V2.h"

V2::V2() {}

V2::V2(float _x, float _y)
	:
	x(_x),
	y(_y)
{}

V2::~V2() {}

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