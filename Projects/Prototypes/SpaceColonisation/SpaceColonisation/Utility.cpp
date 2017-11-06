#define _USE_MATH_DEFINES
#include <math.h>

#include "Utility.h"
#include "Vec2.h"

namespace Utility
{
	float DistanceBetween(Vec2* left, Vec2* right)
	{
		Vec2* vec = new Vec2(left->getX() - right->getX(), left->getY() - right->getY());
		return fabsf(vec->Length());
	}

	// https://stackoverflow.com/questions/14176776/find-out-if-2-lines-intersect
	bool Intersect(Vec2* p1, Vec2* p2, Vec2* q1, Vec2* q2)
	{
		return (((q1->getX() - p1->getX())*(p2->getY() - p1->getY()) - (q1->getY() - p1->getY())*(p2->getX() - p1->getX()))
			* ((q2->getX() - p1->getX())*(p2->getY() - p1->getY()) - (q2->getY() - p1->getY())*(p2->getX() - p1->getX())) < 0)
			&&
			(((p1->getX() - q1->getX())*(q2->getY() - q1->getY()) - (p1->getY() - q1->getY())*(q2->getX() - q1->getX()))
				* ((p2->getX() - q1->getX())*(q2->getY() - q1->getY()) - (p2->getY() - q1->getY())*(q2->getX() - q1->getX())) < 0);
	}
}