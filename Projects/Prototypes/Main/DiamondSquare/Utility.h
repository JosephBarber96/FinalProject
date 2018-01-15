#pragma once
class V2;
namespace Utility
{
	V2* GetIntersectionPointWithFiniteLines(V2* start1, V2* end1, V2* start2, V2* end2);

	V2* GetIntersectionPointBetweenInfiniteSlopes(V2* start1, V2* end1, V2* start2, V2* end2);
}