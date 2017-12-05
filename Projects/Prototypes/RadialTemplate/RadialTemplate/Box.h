#pragma once
class Vec2;
class Box
{
public:
	Box();
	Box(Vec2* tl, Vec2* tr, Vec2* bl, Vec2* br);
	~Box();

	Vec2* topLeft;
	Vec2* topRight;
	Vec2* bottomLeft;
	Vec2* bottomRight;
};

