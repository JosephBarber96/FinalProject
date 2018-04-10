#include "BoundingBox.h"

BoundingBox::BoundingBox() {}

BoundingBox::BoundingBox(int _xOri, int _yOri, int _wid, int _hei)
	:
	xOrigin(_xOri),
	yOrigin(_yOri),
	width(_wid),
	height(_hei)
{}

BoundingBox::~BoundingBox() {}

bool BoundingBox::inBounds(int x, int y)
{
	return (x > xOrigin && x < xOrigin + width && y > yOrigin && y < yOrigin + height);
}