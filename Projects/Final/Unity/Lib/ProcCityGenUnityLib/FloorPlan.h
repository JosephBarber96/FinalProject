#pragma once
#include <vector>

class BoundingBox;
class jbShape;
class shapeLine;
class FloorPlan
{
public:
	FloorPlan();
	~FloorPlan();

	void SetBoundingBox(int xOri, int yOri, int wid, int hei);
	void GenerateShapes(int num);
	void GeneratePerimeter();

	BoundingBox* bb;
	std::vector<jbShape*> shapes;
	std::vector<shapeLine*> perimeterLines;
};