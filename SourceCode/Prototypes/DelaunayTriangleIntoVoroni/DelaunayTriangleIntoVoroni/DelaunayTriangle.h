#pragma once
#include <vector>
class ScreenSegment;
class DelaunayTriangle
{
public:
	DelaunayTriangle();
	~DelaunayTriangle();

	void GenerateDelaunay(const int screenMin, const int screenMax, const int columns, const int rows, bool withDiagonalSplit);
	std::vector<std::vector<ScreenSegment*>> screenSegments;
};

