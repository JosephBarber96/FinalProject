#include "Utility.h"
#include "V2.h"

namespace Utility
{
	V2* GetIntersectionPointWithFiniteLines(V2* start1, V2* end1, V2* start2, V2* end2)
	{
		float p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y;

		p0_x = start1->x;
		p0_y = start1->y;

		p1_x = end1->x;
		p1_y = end1->y;

		p2_x = start2->x;
		p2_y = start2->y;

		p3_x = end2->x;
		p3_y = end2->y;

		// Make sure they arent perfectly vertical
		if (p0_x == p1_x) p0_x += 1;
		if (p2_x == p3_x) p2_x += 1;

		// ---

		float s1_x, s1_y, s2_x, s2_y;
		s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
		s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

		float s, t;
		s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
		t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

		// Collision detexted
		if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		{
			//
			float i_x, i_y;

			i_x = p0_x + (t * s1_x);
			i_y = p0_y + (t * s1_y);

			return new V2(i_x, i_y);
		}
		else
		{
			// No collision
			return nullptr;
		}
	}
}