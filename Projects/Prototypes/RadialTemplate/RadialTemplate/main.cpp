#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <SFML\Graphics.hpp>

#include "Circle.h"
#include "Vec2.h"
#include "Road.h"

int winSize = 700;

int main()
{
	// How large is the bounding box?
	float size = winSize;

	// 1 dial will occur every x meters
	int numOfCircles = size / 80;
	std::vector<Circle> dials;
	int circlePoints = 100;
	for (int i = 0; i < numOfCircles; i++)
	{
		Circle circle;

		// Create the points (normalized) for the circle
		// Refference: https://www.opengl.org/discussion_boards/showthread.php/167955-drawing-a-smooth-circle
		for (int j = 0; j < circlePoints; j++)
		{
			double angle = 2 * M_PI * j / circlePoints;

			double x = cos(angle);
			double y = sin(angle);

			circle.AddPoint(x, y);
		}
		dials.push_back(circle);
	}

	/*
		Space these out evenly, whilst also converting
		into SFML screen space
	*/
	int counter = 1;
	for (Circle &cir : dials)
	{
		for (Vec2* v : cir.points)
		{
			v->x *= (counter * 100);
			v->y *= (counter * 100);

			v->x += winSize / 2;
			v->y += winSize / 2;
		}
		counter++;
	}

	// Create roads between these points
	std::vector<Road> roads;
	for (const Circle &cir : dials)
	{
		for (int i = 0; i < circlePoints - 1; i++)
		{
			Road road;
			road.start = cir.points[i];

			if (i < circlePoints - 2)
			{
				road.end = cir.points[i+1];
			}
			else
			{
				road.end = cir.points[0];
			}

			roads.push_back(road);
		}
	}

	// Create points going outwards
	int outwardsRoads = 5;



	sf::RenderWindow window(sf::VideoMode(winSize, winSize), "Radial template");

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

		// Points
		for (const Circle &circle : dials)
		{
			for (const Vec2* vec : circle.points)
			{
				sf::CircleShape shape;
				shape.setPosition(vec->x, vec->y);
				shape.setRadius(1);
				shape.setFillColor(sf::Color::Yellow);

				window.draw(shape);
			}
		}


		// Roads
		for (const Road &road : roads)
		{
			const int sz = 2;
			sf::Vertex roadPoints[2] =
			{
				sf::Vertex(sf::Vector2f(road.start->x, road.start->y), sf::Color::Blue),
				sf::Vertex(sf::Vector2f(road.end->x, road.end->y), sf::Color::Blue)
			};

			window.draw(roadPoints, sz, sf::LineStrip);
		}

		// Roads
		//for (const Road &road : roads)
		//{
		//	const int sz = 2;
		//	sf::Vertex roadPoints[2] =
		//	{
		//		sf::Vertex(sf::Vector2f(road.start.x, road.start.y), sf::Color::Blue),
		//		sf::Vertex(sf::Vector2f(road.end.x, road.end.y), sf::Color::Blue)
		//	};

		//	window.draw(roadPoints, sz, sf::LineStrip);
		//}

		// Display
		window.display();
	}
	return 0;
}