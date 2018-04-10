#pragma once
#include <vector>
class V2;
class Point
{
public:
	Point();
	Point(int x, int y);
	Point(V2 vec);
	~Point();

	void AddNeighbour(Point* point);

	V2* position;
	std::vector<Point*> neighbours;
};

