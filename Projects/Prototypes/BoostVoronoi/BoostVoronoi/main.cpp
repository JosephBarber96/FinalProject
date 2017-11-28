#include <iostream>
#include <vector>
#include <random>
#include <ctime>

/*
	Requires C++ Boost Library for 1.65s for windows
	https://dl.bintray.com/boostorg/release/1.65.1/binaries/
	(boost_1_65_1-msvc-10.0-32.exe)

	include directory: C:\local\boost_1_65_1
*/
#include<boost\polygon\voronoi.hpp>
#include <SFML\Graphics.hpp>

#include "UtilRandom.h"

using namespace std;
using namespace boost::polygon;

typedef int coordinate_type;
typedef point_data<coordinate_type> point_type;
typedef voronoi_diagram<double> VD;

int winX = 500;
int winY = 500;

int main(int argc, char* argv[])	
{
	srand(time(NULL));

	int difX = (winX / 3);
	int difY = (winY / 3);

	int maxX = winX - difX;
	int minX = 0 + difX;

	int maxY = winY - difY;
	int minY = 0 + difY;

	std::cout << "maxX: " << maxX << " / minxX: " << minX << " / maxY: " << maxY << " / minY " << minY << std::endl;

	vector<point_type> points;
	int pointNum = 25;
	for (int i = 0; i < pointNum; i++)
	{
		double px = UtilRandom::Instance()->Random(minX, maxX);
		double py = UtilRandom::Instance()->Random(minY, maxY);

		std::cout << "Node created on: " << px << ", " << py << std::endl;

		points.push_back(point_type(px, py));
	}

	VD vd;  construct_voronoi(points.begin(), points.end(), &vd);


	sf::RenderWindow window(sf::VideoMode(winX, winY), "Template-based generation - Voronoi");

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

		// vertices
		for (auto const &point : points)
		{
			sf::CircleShape shape;
			shape.setPosition(point.x(), point.y());
			shape.setRadius(1);
			shape.setFillColor(sf::Color::Green);

			window.draw(shape);
		}

		// Edges
		for (auto const &edge : vd.edges())
		{
			if (edge.vertex0() != NULL && edge.vertex1() != NULL)
			{
				const int sz = 2;
				sf::Vertex va[2] =
				{
					sf::Vertex(sf::Vector2f(edge.vertex0()->x(), edge.vertex0()->y()), sf::Color::Red),
					sf::Vertex(sf::Vector2f(edge.vertex1()->x(), edge.vertex1()->y()), sf::Color::Red)
				};

				window.draw(va, sz, sf::LineStrip);

			}


		}

		// Display
		window.display();
	}

	return 0;
}