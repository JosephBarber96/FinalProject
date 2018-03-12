#pragma once
class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(int _xOri, int _yOri, int _wid, int _hei);
	~BoundingBox();

	bool inBounds(int x, int y);

	int xOrigin, yOrigin, width, height;
};