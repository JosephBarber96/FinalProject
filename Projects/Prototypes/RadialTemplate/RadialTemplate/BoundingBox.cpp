#include "BoundingBox.h"
#include "Vec2.h"

BoundingBox::BoundingBox() {}

BoundingBox::BoundingBox(int xOr, int wid, int yOr, int hei)
	:
	xOrigin(xOr),
	width(wid),
	yOrigin(yOr),
	height(hei)
{}

BoundingBox::~BoundingBox() {}

bool BoundingBox::isWithin(float x, float y)
{
	return (x > xOrigin && x < xOrigin + width && y > yOrigin && y < yOrigin + height);
}

bool BoundingBox::isWithin(Vec2* vec)
{
	return (isWithin(vec->x, vec->y));
}