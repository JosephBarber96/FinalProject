#include "jbShape.h"
#include "V2.h"
#include "Line.h"
#include "Intersection.h"

#include "jbTriangle.h"
#include "jbSquare.h"
#include "jbRectangle.h"
#include "jbPentagon.h"
#include "jbHexagon.h"

jbShape::jbShape()
	:
	position(new V2(0, 0))
{}

jbShape::jbShape(V2* pos)
	:
	position(pos)
{}

jbShape::jbShape(V2* pos, std::vector<V2*> shapePoints)
	:
	position(pos),
	points(shapePoints)
{

}

jbShape::~jbShape() {}

void jbShape::SetPosition(int _x, int _y)
{
	ResetShape();

	for (auto point : points)
	{
		point->x += _x;
		point->y += _y;
	}
}

void jbShape::SetPosition(V2 pos)
{
	ResetShape();

	for (auto point : points)
	{
		point->x += pos.x;
		point->y += pos.y;
	}
}

void jbShape::DrawSelf(sf::RenderWindow *window)
{
	sf::VertexArray points(sf::LineStrip, points.size());
	int counter = 0;

	for (auto line : lines)
	{
		sf::Vertex lineVertices[2] =
		{
			sf::Vertex(sf::Vector2f(line->start->x, line->start->y), sf::Color(25, 25, 25)),
			sf::Vertex(sf::Vector2f(line->end->x, line->end->y), sf::Color(25, 25, 25))
		};

		window->draw(lineVertices, 2, sf::LineStrip);

		// Intersections
		for (auto isec : line->intersections)
		{
			sf::CircleShape shape;
			shape.setPosition(isec->point->x - 1, isec->point->y - 1);
			shape.setFillColor(sf::Color::Cyan);
			shape.setRadius(2);

			window->draw(shape);
		}
	}
}

float jbShape::getWidth()
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

float jbShape::getHeight()
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

bool jbShape::PointWithin(int x, int y)
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
	return (x > minX && x < minX + getWidth() && y > minY && y < minY + getHeight());
}

bool jbShape::PointWithin(V2* pos)
{
	return PointWithin(pos->x, pos->y);
}

//! Factory
jbShape* jbShape::CreateShape(Shape shape)
{
	switch (shape)
	{
		// case Shape::triangle:
		// return new jbTriangle();
	case Shape::square:
		return new jbSquare();
	case Shape::rectangle:
		return new jbRectangle();
	case Shape::pentagon:
		return new jbPentagon();
	case Shape::hexagon:
		return new jbHexagon();
	}
}

//! Private
void jbShape::GenerateLines()
{
	lines.clear();

	for (int i = 0; i < points.size() - 1; i++)
	{
		lines.push_back(new Line(points[i], points[i + 1], this));

	}
	lines.push_back(new Line(points[points.size() - 1], points[0], this));
}