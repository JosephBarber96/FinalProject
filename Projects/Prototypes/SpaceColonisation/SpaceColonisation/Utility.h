#pragma once
class Vec2;
namespace Utility
{
	float DistanceBetween(Vec2* left, Vec2* right);

	// https://stackoverflow.com/questions/14176776/find-out-if-2-lines-intersect
	bool Intersect(Vec2* p1, Vec2* p2, Vec2* q1, Vec2* q2);
}
