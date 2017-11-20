#pragma once
class Vec2;
class Line;
namespace Utility
{
	float DegreeToRadians(float degree);
	float DistanceBetween(Vec2* left, Vec2* right);
	Vec2* GetIntersectionPoint(Line* a, Line* b);
}
