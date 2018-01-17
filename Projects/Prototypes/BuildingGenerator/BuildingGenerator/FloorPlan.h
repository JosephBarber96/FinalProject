#pragma once
#include <vector>
#include <map>

#include "SFML\Graphics.hpp"

enum Shape;
class BoundingBox;
class jbShape;
class Line;
class FloorPlan
{
public:
	FloorPlan();
	~FloorPlan();

	void SetBoundingBox(int minx, int miny, int maxx, int maxy);
	void GenerateShapes(int num);
	void GeneratePerimeter();
	void DrawPerimeter(sf::RenderWindow *window);

	BoundingBox* bb;
	std::vector<jbShape*> shapes;
	std::vector<Line*> perimeterLines;
	std::map<Shape, int> shapeCounter;
};