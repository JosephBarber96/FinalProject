#pragma once
class Vec2;
namespace Utility
{
	// General
	template <typename Iter>
	Iter ReturnNextIter(Iter iter) { return ++iter; }

	// City related
	Vec2* GetIntersectionPointWithFiniteLines(Vec2* start1, Vec2* end1, Vec2* start2, Vec2* end2);
	Vec2* GetIntersectionPointBetweenInfiniteSlopes(Vec2* start1, Vec2* end1, Vec2* start2, Vec2* end2);
	Vec2* RotateAroundPoint(Vec2* toRotate, Vec2* pivotPoint, float angle);
}