#include <math.h>

#include "Vec2.h"
#include "Utility.h"

Vec2::Vec2(float newAngle)
{
	Vec2* temp = Vec2::AngleToVector(newAngle);
	x = temp->x;
	y = temp->y;
}

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

Vec2* Vec2::AngleToVector(float degree)
{
	// https://math.stackexchange.com/questions/180874/convert-angle-radians-to-a-heading-vector

	float rads = Utility::DegreeToRadians(degree);

	float x = cos(rads);
	float y = sin(rads);

	float roundedX = roundf(x * 100) / 100;
	float roundedY = roundf(y * 100) / 100;


	return new Vec2(roundedX, roundedY);
}

bool Vec2::operator== (Vec2 vec)
{
	return (this->x == vec.getX() && this->y == vec.getY());
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