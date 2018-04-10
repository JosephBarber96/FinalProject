#pragma once
class Vec2;
class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(int xOr, int wid, int yOr, int hei);
	~BoundingBox();

	bool isWithin(float x, float y);
	bool isWithin(Vec2* vec);

	int xOrigin;
	int yOrigin;
	int width;
	int height;
};