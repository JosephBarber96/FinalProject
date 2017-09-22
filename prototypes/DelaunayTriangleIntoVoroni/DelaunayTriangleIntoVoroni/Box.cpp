#include "Box.h"
Box::Box() {}
Box::Box(int minXArg, int maxXArg, int minYArg, int maxYArg)
	:
	minX(minXArg),
	maxX(maxXArg),
	minY(minYArg),
	maxY(maxYArg)
{}

Box::Box(Box* box)
{
	minX = box->minX;
	maxX = box->maxX;
	minY = box->minY;
	maxY = box->maxY;
}

Box::~Box() {}