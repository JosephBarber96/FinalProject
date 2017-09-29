#include "Point.h"
#include "V2.h"
Point::Point() {}

Point::Point(int x, int y)
{
	position = new V2(x, y);
}

Point::Point(V2 vec)
{
	position = new V2(vec);
}

Point::~Point() {}

void Point::AddNeighbour(Point* point)
{
	neighbours.push_back(point);
}