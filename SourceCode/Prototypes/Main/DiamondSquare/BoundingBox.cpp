#include "BoundingBox.h"

BoundingBox::BoundingBox() {}

BoundingBox::BoundingBox(int _xOri, int _yOri, int _wid, int _hei)
	:
	xOrigin(_xOri),
	yOrigin(_yOri),
	width(_wid),
	height(_hei)
{}

BoundingBox::~BoundingBox() {}

void BoundingBox::DrawSelf(sf::RenderWindow* window)
{
	sf::Vertex vertexArray[5]
	{
		sf::Vertex(sf::Vector2f(xOrigin, yOrigin), sf::Color::Red),
		sf::Vertex(sf::Vector2f(xOrigin, yOrigin + height), sf::Color::Red),
		sf::Vertex(sf::Vector2f(xOrigin + width, yOrigin + height), sf::Color::Red),
		sf::Vertex(sf::Vector2f(xOrigin + width, yOrigin), sf::Color::Red),
		sf::Vertex(sf::Vector2f(xOrigin, yOrigin), sf::Color::Red)
	};

	window->draw(vertexArray, 5, sf::LineStrip);
}

bool BoundingBox::inBounds(int x, int y)
{
	return (x > xOrigin && x < xOrigin + width && y > yOrigin && y < yOrigin + height);
}