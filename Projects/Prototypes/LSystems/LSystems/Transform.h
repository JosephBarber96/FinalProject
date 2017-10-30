#pragma once
class Vec2;
class Transform
{
public:
	Transform();
	Transform(float angle, Vec2* pos);
	~Transform();

	float rotation;
	Vec2* position;
};

