#include "V2.h"
#include <math.h>

V2::V2()
	:
	x(0),
	y(0)
	{}

V2::V2(int newX, int newY)
	:
	x(newX),
	y(newY)
	{}

V2::~V2() {}

float V2::Length()
{
	return sqrt(x * x + y * y);
}

bool V2::operator==(V2 arg)
{
	return (x == arg.x && y == arg.y);
}

// Static methods

float V2::DistanceBetween(V2 left, V2 right)
{
	V2* vec = new V2(left.x - right.x, left.y - right.y);
	return fabsf(vec->Length());
}