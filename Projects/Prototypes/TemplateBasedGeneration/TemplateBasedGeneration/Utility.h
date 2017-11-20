#pragma once
class Vec2;
class Road;
namespace Utility
{
	float DegreeToRadians(float degree);
	float DistanceBetween(Vec2* left, Vec2* right);
	Vec2* GetIntersectionPoint(Vec2* start1, Vec2* end1, Vec2* start2, Vec2* end2);
}
