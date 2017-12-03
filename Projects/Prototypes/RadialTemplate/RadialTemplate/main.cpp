#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <SFML\Graphics.hpp>

int winSize = 500;

struct Vec
{
	float x, y;
};

struct Road
{
	Vec start, end;
};

int main()
{

	// Create the points (normalized) for the circle
	// Refference: https://www.opengl.org/discussion_boards/showthread.php/167955-drawing-a-smooth-circle
	std::vector<Vec> points;
	float circlePoints = 100;
	for (int i = 0; i < circlePoints; i++)
	{
		double angle = 2 * M_PI * i / circlePoints;

		double x = cos(angle);
		double y = sin(angle);

		Vec point;
		point.x = x;
		point.y = y;

		points.push_back(point);
	}

	// Convert these into SFML screen space
	for (Vec &vec : points)
	{
		vec.x *= winSize / 2.5;
		vec.y *= winSize / 2.5;

		vec.x += winSize / 2;
		vec.y += winSize / 2;
	}

	// Create roads between these points
	std::vector<Road> roads;
	for (int i = 0; i < circlePoints-1; i++)
	{
		Road road;
		road.start = points[i];

		if (i < circlePoints-2)
		{
			road.end = points[i + 1];
		}

		/* Loop back around when we get to the last one */
		else
		{
			road.start = points[0];
		}

		roads.push_back(road);
	}


	sf::RenderWindow window(sf::VideoMode(500, 500), "Radial template");

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
		for (const Vec &vec : points)
		{
			sf::CircleShape shape;
			shape.setPosition(vec.x, vec.y);
			shape.setRadius(1);
			shape.setFillColor(sf::Color::Green);

			window.draw(shape);
		}

		// Roads
		for (const Road &road : roads)
		{
			const int sz = 2;
			sf::Vertex roadPoints[2] =
			{
				sf::Vertex(sf::Vector2f(road.start.x, road.start.y), sf::Color::Blue),
				sf::Vertex(sf::Vector2f(road.end.x, road.end.y), sf::Color::Blue)
			};

			window.draw(roadPoints, sz, sf::LineStrip);
		}

		// Display
		window.display();
	}
	return 0;
}