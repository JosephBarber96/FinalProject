#include "jbShape.h"
#include "Vec2.h"
#include "shapeLine.h"
#include "Intersection.h"

#include "jbSquare.h"

jbShape::jbShape() {}

jbShape::~jbShape() {}

void jbShape::SetPosition(float _x, float _y)
{
	ResetShape(shapeSize);

	position = new Vec2(_x, _y);

	for (auto point : points)
	{
		point->x += _x;
		point->y += _y;
	}
}

void jbShape::SetPosition(Vec2 pos)
{
	ResetShape(shapeSize);

	position = new Vec2(pos.x, pos.y);

	for (auto point : points)
	{
		point->x += pos.x;
		point->y += pos.y;
	}
}

float jbShape::GetWidth()
{
	float minX = INT_MAX;
	float maxX = -INT_MAX;

	for (auto point : points)
	{
		if (point->x < minX) minX = point->x;
		if (point->x > maxX) maxX = point->x;
	}

	return fabsf(maxX - minX);
}

float jbShape::GetHeight()
{
	float minY = INT_MAX;
	float maxY = -INT_MAX;

	for (auto point : points)
	{
		if (point->y < minY) minY = point->y;
		if (point->y > maxY) maxY = point->y;
	}

	return fabsf(maxY - minY);
}

bool jbShape::PointWithin(float x, float y)
{
	// Get lowest x and y points point
	float minX = INT_MAX;
	float minY = INT_MAX;
	for (auto point : points)
	{
		if (point->x < minX) minX = point->x;
		if (point->y < minY) minY = point->y;
	}

	// Greter than minX, Less than minX+Width, Greater than minY, Less than minY+Height
	return (x > minX && x < minX + GetWidth() && y > minY && y < minY + GetHeight());
}

bool jbShape::PointWithin(Vec2* pos)
{
	return PointWithin(pos->x, pos->y);
}

//! Factory
jbShape* jbShape::CreateShape(float shapeSize)
{
	return new jbSquare(shapeSize);

	//switch (shape)
	//{
	//	// case Shape::triangle:
	//	// return new jbTriangle();
	//case Shape::square:
	//	return new jbSquare();
	//case Shape::rectangle:
	//	return new jbRectangle();
	//case Shape::pentagon:
	//	return new jbPentagon();
	//case Shape::hexagon:
	//	return new jbHexagon();
	//}
}

//! Private
void jbShape::GenerateLines()
{
	lines.clear();

	for (int i = 0; i < points.size() - 1; i++)
	{
		lines.push_back(new shapeLine(points[i], points[i + 1], this));

	}
	lines.push_back(new shapeLine(points[points.size() - 1], points[0], this));
}