#define _USE_MATH_DEFINES
#include <math.h>

#include "Utility.h"
#include "Vec2.h"

namespace Utility
{
	// General

	// City related

	Vec2* GetIntersectionPointWithFiniteLines(Vec2* start1, Vec2* end1, Vec2* start2, Vec2* end2)
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

		p0_x *= 10;
		p0_y *= 10;
		p1_x *= 10;
		p1_y *= 10;
		p2_x *= 10;
		p2_y *= 10;
		p3_x *= 10;
		p3_y *= 10;

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

			i_x /= 10;
			i_y /= 10;

			return new Vec2(i_x, i_y);
		}
		else
		{
			// No collision
			return nullptr;
		}
	}

	Vec2* GetIntersectionPointBetweenInfiniteSlopes(Vec2* start1, Vec2* end1, Vec2* start2, Vec2* end2)
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

			return new Vec2(intersectionX, intersectionY);
		}
	}

	//! https://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d
	Vec2* RotateAroundPoint(Vec2* toRotate, Vec2* pivotPoint, float angle)
	{
		float s = sin(angle);
		float c = cos(angle);

		// Create a local copy of the vector we are rotating
		Vec2* vector = new Vec2(toRotate->x, toRotate->y);

		// Tranlate vector to the pivot point
		vector->x -= pivotPoint->x;
		vector->y -= pivotPoint->y;

		// Rotate the point
		float rotX = vector->x * c - vector->y * s;
		float rotY = vector->x * s + vector->y * c;

		// Translate the point back
		vector->x = rotX + pivotPoint->x;
		vector->y = rotY + pivotPoint->y;

		return vector;
	}
}