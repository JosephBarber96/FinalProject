#define _USE_MATH_DEFINES
#include <math.h>

#include "Utility.h"
#include "Vec2.h"
#include "Road.h"

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
	Vec2* GetIntersectionPointWithSlope(Vec2* start1, Vec2* end1, Vec2* start2, Vec2* end2)
	{
		float m1, c1, m2, c2;
		float x1, y1, x2, y2;
		float dx, dy;
		float intersectionX, intersectionY;

		// Get the values for line 1
		x1 = start1->getX();
		y1 = start1->getY();
		x2 = end1->getX();
		y2 = end1->getY();

		dx = x2 - x1;
		dy = y2 - y1;

		m1 = dy / dx;
		c1 = y1 - m1 * x1;

		// Get the values for line2;
		x1 = start2->getX();
		y1 = start2->getY();
		x2 = end2->getX();
		y2 = end2->getY();

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

	//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
	Vec2* GetLineIntersectionPointWithFiniteLine(Vec2* line1Start, Vec2* line1End, Vec2* line2Start, Vec2* line2End)
	{
		float p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y;

		p0_x = line1Start->getX();
		p0_y = line1Start->getY();

		p1_x = line1End->getX();
		p1_y = line1End->getY();

		p2_x = line2Start->getX();
		p2_y = line2Start->getY();

		p3_x = line2End->getX();
		p3_y = line2End->getY();

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

			return new Vec2(i_x, i_y);
		}
		else
		{
			// No collision
			return nullptr;
		}
	}
}