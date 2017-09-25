#include "ScreenSegment.h"
ScreenSegment::ScreenSegment() {}
ScreenSegment::ScreenSegment(int minXArg, int maxXArg, int minYArg, int maxYArg)
	:
	minX(minXArg),
	maxX(maxXArg),
	minY(minYArg),
	maxY(maxYArg)
{}

ScreenSegment::ScreenSegment(ScreenSegment* segment)
{
	minX = segment->minX;
	maxX = segment->maxX;
	minY = segment->minY;
	maxY = segment->maxY;
}

ScreenSegment::~ScreenSegment() {}