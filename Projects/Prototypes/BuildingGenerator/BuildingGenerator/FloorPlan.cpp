#include "FloorPlan.h"
#include "V2.h"
#include "Line.h"
#include "BoundingBox.h"
#include "UtilRandom.h"

#include "jbShape.h"
#include "jbTriangle.h"
#include "jbSquare.h"
#include "jbPentagon.h"

FloorPlan::FloorPlan()
{
	shapeCounter[Shape::square] = 0;
	shapeCounter[Shape::rectangle] = 0;
	shapeCounter[Shape::pentagon] = 0;
	shapeCounter[Shape::hexagon] = 0;
}

FloorPlan::~FloorPlan() {}

void FloorPlan::SetBoundingBox(int minx, int miny, int maxx, int maxy)
{
	bb = new BoundingBox(minx, miny, maxx, maxy);
}

void FloorPlan::GenerateShapes(int num)
{
	std::vector<int> limits =
	{
		2, // square
		2, // rectangle
		1, // pentagon
		1, // hexagon
	};

	for (int i = 0; i < num; i++)
	{
		jbShape* shape;
		int index;
		do
		{
			// Create a random shape
			index = UtilRandom::Instance()->Random(0, Shape::SHAPE_MAX);
			shape = jbShape::CreateShape(Shape(index));
		}
		// While this shape hasn't met its limit
		while (shapeCounter[Shape(index)] >= limits[index]);

		// We now have one more of this shape
		shapeCounter[Shape(index)]++;

		// Place the shape randomly within the bounds of the bounding box
		float shapeX = UtilRandom::Instance()->Random(bb->minX, bb->maxX - shape->getWidth());
		float shapeY = UtilRandom::Instance()->Random(bb->minY, bb->maxY - shape->getHeight());
		shape->SetPosition(shapeX, shapeY);

		// Add the shape to our floor plan
		shapes.push_back(shape);
	}
}