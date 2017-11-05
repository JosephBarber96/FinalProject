#pragma once
class Vec2;
namespace Utility
{
	float DistanceBetween(Vec2* left, Vec2* right);

	// http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
	bool OnSegment(Vec2* p, Vec2* q, Vec2* r);
	int Orientation(Vec2* p, Vec2* q, Vec2* r);
	bool DoIntersect(Vec2* p1, Vec2* q1, Vec2* p2, Vec2* q2);

	
}
