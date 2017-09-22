#pragma once
#include <vector>
class Point;
class Box;
class DelaunayTriangle
{
public:
	DelaunayTriangle();
	~DelaunayTriangle();

	void Generate(int size);

	std::vector<Point*> points;
	std::vector<Box> screenSegments;
};

