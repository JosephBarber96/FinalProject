#include <iostream>
#include <vector>

#include <SFML\Graphics.hpp>

#include "Road.h"
#include "MinorRoad.h"
#include "Vec2.h"
#include "Utility.h"

std::vector<Vec2*> intersectionPoints;
int minX, minY, maxX, maxY;

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "Template-based generation");

	Road* road;

	road = new Road(100, 100, 120, 400);
	road = new Road(120, 400, 410, 380);
	road = new Road(410, 380, 400, 120);
	road = new Road(400, 120, 100, 100);

	float distBetweenRoads = 14.f;
	float angle = 13.f;

	Road::getRoads()[0]->CreateMinorRoads(distBetweenRoads, angle);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) { window.close(); }
		}

		// Clear
		window.clear();

		// Draw

		// Lines
		for (Road* r : Road::getRoads())
		{
			const int sz = 2;
			sf::Vertex linePoints[sz] =
			{
				sf::Vertex(sf::Vector2f(r->start->getX(), r->start->getY()), sf::Color::Red),
				sf::Vertex(sf::Vector2f(r->end->getX(), r->end->getY()), sf::Color::Red)
			};

			window.draw(linePoints, sz, sf::LineStrip);
		}

		for (MinorRoad* mr : MinorRoad::getMinorRoads())
		{
			const int sz = 2;
			sf::Vertex linePoints[sz] =
			{
				sf::Vertex(sf::Vector2f(mr->start->getX(), mr->start->getY()), sf::Color::Blue),
				sf::Vertex(sf::Vector2f(mr->end->getX(), mr->end->getY()), sf::Color::Blue)
			};

			window.draw(linePoints, sz, sf::LineStrip);
		}

		// Intersection points
		for (Vec2* intersec : intersectionPoints)
		{
			sf::CircleShape shape(1.5f);
			shape.setFillColor(sf::Color::Green);
			shape.setPosition(intersec->getX(), intersec->getY());

			window.draw(shape);
		}

		// Display
		window.display();
	}

	return 0;
}