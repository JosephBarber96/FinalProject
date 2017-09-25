#pragma once
#include <vector>
class ScreenSegment;
class DelaunayTriangle
{
public:
	DelaunayTriangle();
	~DelaunayTriangle();

	void GenerateDelaunay(const int screenSize, const int columns, const int rows, bool withDiagonalSplit);
	int columns;
	int rows;

	std::vector<std::vector<ScreenSegment*>> screenSegments;
};

