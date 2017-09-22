#pragma once
class Box
{
public:
	Box();
	Box(int minXArg, int maxXArg, int minYArg, int maxYArg);
	Box(Box* box);
	~Box();

	int minX, maxX, minY, maxY;
};

