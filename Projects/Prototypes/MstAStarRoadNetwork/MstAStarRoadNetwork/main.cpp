#include <array>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <map>
#include <iostream>

#include <SFML\Graphics.hpp>

#include "MinimumSpanningTree.h"
#include "Edge.h"
#include "Node.h"
#include "V2.h"
#include "QuadTree.h"
#include "debug_mode.h"

int winX;
int winY;

MinimumSpanningTree* mst;

void SearchImageForPixelData(sf::Image image)
{
	int xSize = image.getSize().x;
	int ySize = image.getSize().y;

	int counter = 0;
	std::map<int, int> colorMap;

	for (int y = 0; y < ySize; y++)
	{
		for (int x = 0; x < xSize; x++)
		{
			// Grab the int32 representation of the color
			auto color = image.getPixel(x, y).toInteger();

			if (colorMap.find(color) == colorMap.end())
			{
				colorMap.insert(std::pair<int, int>(color, counter));
				counter++;
			}
			else
			{
				colorMap[color]++;
			}
		}
	}

	std::cout << "Number of different pixel colours:" << colorMap.size() << std::endl;
	std::map<int, int>::iterator iter;
	for (iter = colorMap.begin(); iter != colorMap.end(); iter++)
	{
		std::cout << "Colour key: " << iter->first << " entries: " << iter->second << std::endl;
	}
}

int main()
{
	// Seed random
	srand(time(NULL));

	// Read in population data
	std::string populationMapFileName = "PopulationMap.bmp";
	sf::Image populationMap;
	if (!populationMap.loadFromFile(populationMapFileName))
	{
		std::cerr << "Could not find/open populationMap" << std::endl;
		return -1;
	}

	// Create a window
	winX = populationMap.getSize().x;
	winY = populationMap.getSize().y;
	sf::RenderWindow window(sf::VideoMode(winX, winY), "Population Data Quad Tree");

	// Create a sprite to display this image;
	sf::Texture populationTexture;
	if (!populationTexture.loadFromFile(populationMapFileName))
	{
		return -1;
	}
	populationTexture.setSmooth(true);

	sf::Sprite populationSprite;
	populationSprite.setTexture(populationTexture);
	populationSprite.setPosition(0, 0);

#if DEBUG_MODE == true
	SearchImageForPixelData(populationMap);
#endif

	// Create a quad tree
	QuadTree* qt = new QuadTree(0, 0, winX, winY, populationMap);

	// Minimum Spanning tree
	mst = new MinimumSpanningTree();

	// Plot points
	for (QuadTree* qt : QuadTree::Children)
	{
		std::cout << "minX: " << qt->xOrigin << ", minY: " << qt->yOrigin << ", maxX: " << (qt->xOrigin + qt->width) << ", maxY: " << (qt->yOrigin + qt->height) << std::endl;

		mst->SpawnPoint(qt->xOrigin, qt->yOrigin, (qt->xOrigin + qt->width), (qt->yOrigin + qt->height));
	}

	// Assign neighbour
	float maxDist = winX / 4;
	mst->AssignNighbours(maxDist);

	// Edges
	mst->CreateAllEdges();

	// Sort the MST
	mst->Sort();

	std::cout << "The complete MST has " << mst->GetTreeEdges().size() << " edges.";

	// Display
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) { window.close(); }
		}

		// Clear
		window.clear();

		// Draw...

		// Population map
		window.draw(populationSprite);

		// Quad tree
		for (QuadTree* qt : QuadTree::Children)
		{
			sf::Vertex qtVertices[5] =
			{
				sf::Vertex(sf::Vector2f(qt->xOrigin, qt->yOrigin), sf::Color::Red),
				sf::Vertex(sf::Vector2f(qt->xOrigin, qt->yOrigin + qt->height), sf::Color::Red),
				sf::Vertex(sf::Vector2f(qt->xOrigin + qt->width, qt->yOrigin + qt->height), sf::Color::Red),
				sf::Vertex(sf::Vector2f(qt->xOrigin + qt->width, qt->yOrigin), sf::Color::Red),
				sf::Vertex(sf::Vector2f(qt->xOrigin, qt->yOrigin), sf::Color::Red)
			};

			window.draw(qtVertices, 5, sf::LineStrip);
		}

		// MST
		for (Edge edge : mst->GetTreeEdges())
		{
			sf::Vertex edgeVertices[2] =
			{
				sf::Vertex(sf::Vector2f(edge.start->position->x, edge.start->position->y), sf::Color::Blue),
				sf::Vertex(sf::Vector2f(edge.end->position->x, edge.end->position->y), sf::Color::Blue)
			};

			window.draw(edgeVertices, 2, sf::Lines);
		}

		// Nodes
		for (Node* node : mst->GetNodes())
		{
			sf::CircleShape point(3);
			point.setFillColor(sf::Color::Green);
			point.setPosition(node->position->x -1.5f, node->position->y -1.5f);

			window.draw(point);

			//sf::Vertex nodeVertiex[1] =
			//{
			//	sf::Vertex(sf::Vector2f(node->position->x, node->position->y), sf::Color::Green)
			//};

			//window.draw(nodeVertiex, 1, sf::Points);
		}

		// Display
		window.display();
	}


	return 0;
}