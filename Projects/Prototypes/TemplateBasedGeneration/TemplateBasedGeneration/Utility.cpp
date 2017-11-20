#define _USE_MATH_DEFINES
#include <math.h>

#include "Utility.h"
#include "Vec2.h"
#include "Line.h"

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

	//! http://www.softwareandfinance.com/Visual_CPP/VCPP_Intersection_Two_lines_EndPoints.html
	Vec2* GetIntersectionPoint(Line* a, Line* b)
	{
		float m1, c1, m2, c2;
		float x1, y1, x2, y2;
		float dx, dy;
		float intersectionX, intersectionY;

		// Get the values for line 1
		x1 = a->start->getX();
		y1 = a->start->getY();
		x2 = a->end->getX();
		y2 = a->end->getY();

		dx = x2 - x1;
		dy = y2 - y1;

		m1 = dy / dx;
		c1 = y1 - m1 * x1;

		// Get the values for line2;
		x1 = b->start->getX();
		y1 = b->start->getY();
		x2 = b->end->getX();
		y2 = b->end->getY();

		dx = x2 - x1;
		dy = y2 - y1;

		m2 = dy / dx;
		c2 = y2 - m2 * x2;

		if ((m1 - m2) == 0)
		{
			return nullptr;
		}
		else
		{
			intersectionX = (c2 - c1) / (m1 - m2);
			intersectionY = m1 * intersectionX + c1;

			return new Vec2(intersectionX, intersectionY);
		}
	}
}