#pragma once
class Point;
class ScreenSegment
{
public:
	ScreenSegment();
	ScreenSegment(int minXArg, int maxXArg, int minYArg, int maxYArg);
	ScreenSegment(ScreenSegment* segment);
	~ScreenSegment();

	int minX, maxX, minY, maxY;
	Point* point;
};

