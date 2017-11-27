#include <iostream>
#include <vector>
#include <random>
#include <ctime>

#include <SFML\Graphics.hpp>
#include<boost\polygon\voronoi.hpp>

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

	vector<point_type> points;
	int pointNum = 25;
	for (int i = 0; i < pointNum; i++)
	{
		double px = rand() % winX - 0;
		double py = rand() % winY - 0;

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