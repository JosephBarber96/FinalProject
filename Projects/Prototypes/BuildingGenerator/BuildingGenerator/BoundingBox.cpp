#include "BoundingBox.h"

BoundingBox::BoundingBox() {}

BoundingBox::BoundingBox(int minx, int miny, int maxx, int maxy)
	:
	minX(minx),
	minY(miny),
	maxX(maxx),
	maxY(maxy)
{}

BoundingBox::~BoundingBox() {}

void BoundingBox::DrawSelf(sf::RenderWindow* window)
{
	sf::Vertex vertexArray[5]
	{
		sf::Vertex(sf::Vector2f(minX, minY), sf::Color::Red),
		sf::Vertex(sf::Vector2f(minX, maxY), sf::Color::Red),
		sf::Vertex(sf::Vector2f(maxX, maxY), sf::Color::Red),
		sf::Vertex(sf::Vector2f(maxX, minY), sf::Color::Red),
		sf::Vertex(sf::Vector2f(minX, minY), sf::Color::Red)
	};

	window->draw(vertexArray, 5, sf::LineStrip);
}