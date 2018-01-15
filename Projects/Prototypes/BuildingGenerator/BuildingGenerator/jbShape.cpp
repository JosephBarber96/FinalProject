#include "jbShape.h"
#include "V2.h"
#include "Line.h"

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
	position = new V2(_x, _y);
}

void jbShape::SetPosition(V2 pos)
{
	position = new V2(pos.x, pos.y);
}

void jbShape::DrawSelf(sf::RenderWindow *window)
{
	sf::VertexArray points(sf::LineStrip, points.size());
	int counter = 0;

	for (auto line : lines)
	{
		sf::Vertex lineVertices[2] =
		{
			sf::Vertex(sf::Vector2f(line->start->x + position->x, line->start->y + position->y), sf::Color::White),
			sf::Vertex(sf::Vector2f(line->end->x + position->x, line->end->y + position->y), sf::Color::White)
		};

		window->draw(lineVertices, 2, sf::LineStrip);
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
	for (int i = 0; i < points.size() - 1; i++)
	{
		lines.push_back(new Line(points[i], points[i + 1]));

	}
	lines.push_back(new Line(points[points.size() - 1], points[0]));
}