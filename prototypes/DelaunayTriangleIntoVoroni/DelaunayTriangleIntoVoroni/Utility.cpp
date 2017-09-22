#include "Utility.h"
#include "V2.h"
#include <math.h>

float Utility::Length(const V2 vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

float Utility::DistanceBetween(const V2 left, const V2 right)
{
	V2 vec = V2(left.x - right.x, left.y - right.y);
	float vecLength = Length(vec);

	return fabsf(vecLength);
	/*
	float leftDist = Length(left);
	float rightDist = Length(right);

	float dist = fabsf(leftDist - rightDist);
	return dist;*/
}