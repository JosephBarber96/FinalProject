#define _USE_MATH_DEFINES
#include <math.h>

#include "V2.h"


V2::V2()
	:
	x(0),
	y(0)
{}

V2::V2(float newX, float newY)
	:
	x(newX),
	y(newY)
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

V2* V2::Normalized()
{
	float length = V2::Length(*this);
	if (length != 0)
	{
		float newX = x / length;
		float newY = y / length;

		return new V2(newX, newY);
	}
	return nullptr;
}

bool V2::operator==(V2 arg)
{
	return (x == arg.x && y == arg.y);
}

V2 V2::operator- (V2 arg)
{
	return V2(x - arg.x, y - arg.y);
}

// Static methods

float V2::DistanceBetween(V2 left, V2 right)
{
	V2* vec = new V2(left.x - right.x, left.y - right.y);
	return fabsf(vec->Length());
}

float V2::DegreeToRadians(float degree)
{
	return (float)degree * M_PI / 180;
}

V2* V2::AngleToVector(float degree)
{	
	// https://math.stackexchange.com/questions/180874/convert-angle-radians-to-a-heading-vector

	float rads = V2::DegreeToRadians(degree);

	float x = cos(rads);
	float y = sin(rads);

	float roundedX = roundf(x * 100) / 100;
	float roundedY = roundf(y * 100) / 100;


	return new V2(roundedX, roundedY);
}

float V2::VectorToAngle(V2* vec)
{
	float rads = atan2f(vec->y, vec->x);
	float deg = rads * 180 / M_PI;
	return deg;
}