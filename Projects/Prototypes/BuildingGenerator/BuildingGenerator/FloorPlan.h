#pragma once
#include <vector>
#include <map>

enum Shape;
class BoundingBox;
class jbShape;
class FloorPlan
{
public:
	FloorPlan();
	~FloorPlan();

	void SetBoundingBox(int minx, int miny, int maxx, int maxy);
	void GenerateShapes(int num);

	BoundingBox* bb;
	std::vector<jbShape*> shapes;
	std::map<Shape, int> shapeCounter;
};