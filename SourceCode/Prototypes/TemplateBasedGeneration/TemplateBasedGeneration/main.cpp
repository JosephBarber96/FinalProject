#include <iostream>
#include <vector>

#include <SFML\Graphics.hpp>

#include "Road.h"
#include "MinorRoad.h"
#include "MajorRoad.h"
#include "Road.h"
#include "Vec2.h"
#include "Utility.h"

std::vector<Vec2*> intersectionPoints;
int minX, minY, maxX, maxY;

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "Template-based generation");

	MajorRoad* road;

	// Create Major roads
	road = new MajorRoad(90, 110, 125, 300);
	road = new MajorRoad(125, 300, 225, 450);
	road = new MajorRoad(225, 450, 400, 190);
	road = new MajorRoad(400, 190, 300, 90);
	road = new MajorRoad(300, 90, 90, 110);

	// Square
	//road = new Road(100, 100, 120, 400);
	//road = new Road(120, 400, 410, 380);
	//road = new Road(410, 380, 400, 120);
	//road = new Road(400, 120, 100, 100);

	// Specify minor road spawning variables
	float distBetweenRoads = 15.f;
	float angle = 24.f;

	// Spawn minor roads
	MajorRoad::getRoads()[0]->CreateMinorRoads(distBetweenRoads, angle);
	MajorRoad::getRoads()[1]->CreateMinorRoads(distBetweenRoads, angle);
	MajorRoad::getRoads()[4]->CreateMinorRoads(distBetweenRoads, angle);

	// Now that we have all of our minor roads, branch
	float distBetweenMinorRoads = 13.f;
	for (auto road : MinorRoad::getMinorRoads())
	{
		road->Branch(distBetweenMinorRoads);
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

		// Roads

		// Minor roads
		for (MinorRoad* mr : MinorRoad::getMinorRoads())
		{
			// Draw the minor roads
			const int sz = 2;
			sf::Vertex linePoints[sz] =
			{
				sf::Vertex(sf::Vector2f(mr->start->getX(), mr->start->getY()), sf::Color::Blue),
				sf::Vertex(sf::Vector2f(mr->end->getX(), mr->end->getY()), sf::Color::Blue)
			};

			// Draw its branches
			for (MinorRoad* branch : mr->getBranches())
			{

				if (branch->end == nullptr)
				{
					sf::CircleShape shape(1.5f);
					shape.setFillColor(sf::Color::Cyan);
					shape.setPosition(branch->start->getX(), branch->start->getY());

					window.draw(shape);
				}

				else
				{
					const int sz = 2;
					sf::Vertex branchPoints[sz] =
					{
						sf::Vertex(sf::Vector2f(branch->start->getX(), branch->start->getY()), sf::Color::Blue),
						sf::Vertex(sf::Vector2f(branch->end->getX(), branch->end->getY()), sf::Color::Blue)
					};

					window.draw(branchPoints, sz, sf::LineStrip);
				}

			}

			window.draw(linePoints, sz, sf::LineStrip);
		}

		// Major roads
		for (MajorRoad* r : MajorRoad::getRoads())
		{
			const int sz = 2;
			sf::Vertex linePoints[sz] =
			{
				sf::Vertex(sf::Vector2f(r->start->getX(), r->start->getY()), sf::Color::Red),
				sf::Vertex(sf::Vector2f(r->end->getX(), r->end->getY()), sf::Color::Red)
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