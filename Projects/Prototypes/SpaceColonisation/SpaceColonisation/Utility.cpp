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

	// http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
	bool OnSegment(Vec2* p, Vec2* q, Vec2* r)
	{
		if (q->getX() <=  fmax(q->getX(), r->getX()) && q->getX() >= fmin(p->getX(), r->getX()) &&
			q->getY() <= fmax(p->getY(), r->getY()) && q->getY() >= fmin(p->getY(), r->getY()))
		{
			return true;
		}

		return false;
	}

	// http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
	int Orientation(Vec2* p, Vec2* q, Vec2* r)
	{
		int val = (q->getY() - p->getY()) * (r->getX() - q->getX()) -
			(q->getX() - p->getX()) * (r->getY() - q->getY());

		if (val == 0) { return 0; }

		return (val > 0) ? 1 : 2;
	}

	// http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
	bool DoIntersect(Vec2* p1, Vec2* q1, Vec2* p2, Vec2* q2)
	{
		int o1 = Orientation(p1, q1, p2);
		int o2 = Orientation(p1, q1, q2);
		int o3 = Orientation(p2, q2, p1);
		int o4 = Orientation(p2, q2, p1);

		if (o1 != o2 && o3 != o4) { return true; }

		if (o1 == 0 && OnSegment(p1, p2, q1)) { return true; }

		if (o2 == 0 && OnSegment(p1, q2, q1)) { return true; }

		if (o3 == 0 && OnSegment(p2, p1, q2)) { return true; }

		if (o4 == 0 && OnSegment(p2, q1, q2)) { return true; }

		return false;
	}
}