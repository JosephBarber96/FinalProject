#define _USE_MATH_DEFINES
#include <math.h>

#include "Vec2.h"

Vec2::Vec2()
	:
	x(0),
	y(0)
{}

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

float Vec2::Length(Vec2 vec)
{
	return (float)sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

Vec2* Vec2::Normalized()
{
	float length = Vec2::Length(*this);
	if (length != 0)
	{
		float newX = x / length;
		float newY = y / length;

		return new Vec2(newX, newY);
	}
	return nullptr;
}

bool Vec2::operator==(Vec2 arg)
{
	return (x == arg.x && y == arg.y);
}

bool Vec2::operator!=(Vec2 arg)
{
	if (x == arg.x && y == arg.y)
		return false;
	return true;
}

Vec2 Vec2::operator- (Vec2 arg)
{
	return Vec2(x - arg.x, y - arg.y);
}

// Static methods

float Vec2::DistanceBetween(Vec2 left, Vec2 right)
{
	Vec2 vec = Vec2(left.x - right.x, left.y - right.y);
	return fabsf(vec.Length());
}

float Vec2::DegreeToRadians(float degree)
{
	return (float)degree * M_PI / 180;
}

Vec2* Vec2::AngleToVector(float degree)
{
	// https://math.stackexchange.com/questions/180874/convert-angle-radians-to-a-heading-vector

	float rads = Vec2::DegreeToRadians(degree);

	float x = cos(rads);
	float y = sin(rads);

	float roundedX = roundf(x * 100) / 100;
	float roundedY = roundf(y * 100) / 100;

	return new Vec2(roundedX, roundedY);
}

float Vec2::VectorToAngle(Vec2* vec)
{
	float rads = atan2f(vec->y, vec->x);
	float deg = rads * 180 / M_PI;
	return deg;
}

float Vec2::VectorToAngle(Vec2 vec)
{
	float rads = atan2f(vec.y, vec.x);
	float deg = rads * 180 / M_PI;
	return deg;
}