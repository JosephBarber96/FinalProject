#pragma once
class Vec2;
class MajorRoad;
namespace Utility
{
	float DegreeToRadians(float degree);
	float DistanceBetween(Vec2* left, Vec2* right);
	Vec2* GetIntersectionPoint(Vec2* start1, Vec2* end1, Vec2* start2, Vec2* end2);

	//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
	Vec2* New_GetLineIntersectionPoint(Vec2* line1Start, Vec2* line1End, Vec2* line2Start, Vec2* line2End);
}
