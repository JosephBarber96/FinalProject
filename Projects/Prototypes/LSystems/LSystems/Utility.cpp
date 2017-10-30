#define _USE_MATH_DEFINES
#include <math.h>

#include "Utility.h"
#include "Vec2.h"

namespace Utility
{
	float DegreeToRadians(float degree)
	{
		return (float)degree * M_PI / 180;
	}

	float DistanceBetween(Vec2* left, Vec2* right)
	{
		Vec2* vec = new Vec2(left->getX() - right->getX(), left->getY() - right->getY());
		return fabsf(vec->Length());
	}
}