#include <iostream>
#include <vector>

#include <SFML\Graphics.hpp>

#include "Line.h"
#include "Vec2.h"
#include "Utility.h"

std::vector<Line*> majorLines;
std::vector<Line*> minorLines;
std::vector<Vec2*> intersectionPoints;


int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "Template-based generation");

	majorLines.push_back(new Line(100, 100, 400, 400));
	majorLines.push_back(new Line(200, 100, 400, 300));

	minorLines.push_back(new Line(100, 400, 400, 100));

	for (auto majorLine : majorLines)
	{
		for (auto minorLine : minorLines)
		{
			Vec2* ip = Utility::GetIntersectionPoint(majorLine, minorLine);

			if (ip != nullptr) intersectionPoints.push_back(ip);
		}
	}


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
		for (Line* line : majorLines)
		{
			const int sz = 2;
			sf::Vertex linePoints[sz] =
			{
				sf::Vertex(sf::Vector2f(line->start->getX(), line->start->getY()), sf::Color::Red),
				sf::Vertex(sf::Vector2f(line->end->getX(), line->end->getY()), sf::Color::Red)
			};

			window.draw(linePoints, sz, sf::LineStrip);
		}

		for (Line* line : minorLines)
		{
			const int sz = 2;
			sf::Vertex linePoints[sz] =
			{
				sf::Vertex(sf::Vector2f(line->start->getX(), line->start->getY()), sf::Color::Blue),
				sf::Vertex(sf::Vector2f(line->end->getX(), line->end->getY()), sf::Color::Blue)
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