#include <vector>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <limits>

#include <SFML\Graphics.hpp>

// https://github.com/mdally/Voronoi

#include "Point2.h"
#include "Vector2.h"
#include "VoronoiDiagramGenerator.h"

const int windowWidth = 500;
const int windowHeight = 500;

double normalize(double in, int dimension)
{
	return in / (float)dimension * 1.8 - 0.9;
}

// globals for use in giving relaxation commands
int relax = 0;
bool startOver = true;
bool relaxForever = false;

bool sitesOrdered(const Point2& s1, const Point2& s2)
{
	if (s1.y < s2.y) { return true; }
	else if (s1.y == s2.y && s1.x < s2.x) { return true; }
	else { return false; }
}

void genRandomSites(std::vector<Point2>& sites, BoundingBox& bbox, unsigned int dimension, unsigned int numSites) {
	bbox = BoundingBox(0, dimension, dimension, 0);
	std::vector<Point2> tmpSites;

	tmpSites.reserve(numSites);
	sites.reserve(numSites);

	Point2 s;

	srand(std::clock());
	for (unsigned int i = 0; i < numSites; ++i) {
		s.x = 1 + (rand() / (double)RAND_MAX)*(dimension - 2);
		s.y = 1 + (rand() / (double)RAND_MAX)*(dimension - 2);
		tmpSites.push_back(s);
	}

	//remove any duplicates that exist
	std::sort(tmpSites.begin(), tmpSites.end(), sitesOrdered);
	sites.push_back(tmpSites[0]);
	for (Point2& s : tmpSites) {
		if (s != sites.back()) sites.push_back(s);
	}
}

int main()
{
	unsigned int nPoints;
	unsigned dimension = 1000000;
	VoronoiDiagramGenerator vdg = VoronoiDiagramGenerator();
	Diagram* diagram = nullptr;

	std::vector<Point2>* sites;
	BoundingBox bbox;


	// Setup
	sites = new std::vector<Point2>();
	nPoints = 50;
	genRandomSites(*sites, bbox, dimension, nPoints);
	diagram = vdg.compute(*sites, bbox);

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Template-based generation - Voronoi");

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
		for (Cell* cell : diagram->cells)
		{
			Point2& p = cell->site.p;

			double pX, pY;

			pX = normalize(p.x, dimension);
			pY = -normalize(p.y, dimension);

			//pX = p.x;
			//pY = p.y;

			pX *= (windowHeight / 2);
			pX += (windowHeight / 2);

			pY *= (windowHeight / 2);
			pY += (windowHeight / 2);

			sf::CircleShape shape;
			shape.setRadius(1);
			shape.setPosition(sf::Vector2f(pX, pY));
			shape.setFillColor(sf::Color::Green);

			std::cout << "Shape pos: " << shape.getPosition().x << ", " << shape.getPosition().y << std::endl;

			window.draw(shape);
		}

		// Edges
		for (Edge* edge : diagram->edges)
		{
			if (edge->vertA && edge->vertB)
			{
				Point2& p1 = *edge->vertA;
				Point2& p2 = *edge->vertB;

				double p1_0, p1_1, p2_0, p2_1;

				p1_0 = normalize(p1[0], dimension);
				p1_1 = -normalize(p1[1], dimension);
				p2_0 = normalize(p2[0], dimension);
				p2_1 = -normalize(p2[1], dimension);
				

				p1_0 *= (windowHeight / 2);
				p1_0 += (windowHeight / 2);

				p1_1 *= (windowHeight / 2);
				p1_1 += (windowHeight / 2);

				p2_0 *= (windowHeight / 2);
				p2_0 += (windowHeight / 2);

				p2_1 *= (windowHeight / 2);
				p2_1 += (windowHeight / 2);

				//std::cout << "Edge p1x: " << p1x << " p1y: " << p1y << " p2x: " << p2x << " p2y: " << p2y << std::endl;

				const int sz = 2;
				sf::Vertex va[sz] =
				{
					sf::Vertex(sf::Vector2f(p1_0, p1_1), sf::Color::Red),
					sf::Vertex(sf::Vector2f(p2_0, p2_1), sf::Color::Red)
					//sf::Vertex(sf::Vector2f(normalize(p1[0], dimension), -normalize(p1[1], dimension)), sf::Color::Red),
					//sf::Vertex(sf::Vector2f(normalize(p2[0], dimension), -normalize(p2[1], dimension)), sf::Color::Red)
				};

				window.draw(va, sz, sf::LineStrip);
			}
		}

		
		// DIsplay
		window.display();
	}

	return 0;
}