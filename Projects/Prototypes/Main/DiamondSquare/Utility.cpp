#define _USE_MATH_DEFINES
#include <math.h>

#include "Utility.h"
#include "V2.h"

namespace Utility
{
	V2* GetIntersectionPoint(V2* start1, V2* end1, V2* start2, V2* end2)
	{
		float m1, c1, m2, c2;
		float x1, y1, x2, y2;
		float dx, dy;
		float intersectionX, intersectionY;

		// Get the values for line 1
		x1 = start1->x;
		y1 = start1->y;
		x2 = end1->x;
		y2 = end1->y;

		dx = x2 - x1;
		dy = y2 - y1;

		m1 = dy / dx;
		c1 = y1 - m1 * x1;

		// Get the values for line2;
		x1 = start2->x;
		y1 = start2->y;
		x2 = end2->x;
		y2 = end2->y;

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

			return new V2(intersectionX, intersectionY);
		}
	}
}