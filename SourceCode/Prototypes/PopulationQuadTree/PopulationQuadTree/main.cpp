#include <array>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <map>
#include <iostream>

#include <SFML\Graphics.hpp>

#include "QuadTree.h"

int winX;
int winY;

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
	std::string filename = "PopulationMap.bmp";

	// Read in population data
	sf::Image populationMap;
	if (!populationMap.loadFromFile(filename))
	{
		return -1;
	}

	winX = populationMap.getSize().x;
	winY = populationMap.getSize().y;

	// Create a window
	sf::RenderWindow window(sf::VideoMode(winX, winY), "Population Data Quad Tree");

	// Create a sprite to display this image;
	sf::Texture populationTexture;
	if (!populationTexture.loadFromFile(filename))
	{
		return -1;
	}
	populationTexture.setSmooth(true);

	sf::Sprite populationSprite;
	populationSprite.setTexture(populationTexture);
	populationSprite.setPosition(0, 0);

	SearchImageForPixelData(populationMap);

	// Create a quad tree
	QuadTree* qt = new QuadTree(0, 0, winX, winY, populationMap);

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

		// Draw
		window.draw(populationSprite);

		for (QuadTree* qt : QuadTree::Children)
		{
			sf::Vertex vertices[5] =
			{
				sf::Vertex(sf::Vector2f(qt->xOrigin, qt->yOrigin), sf::Color::Red),
				sf::Vertex(sf::Vector2f(qt->xOrigin, qt->yOrigin + qt->height), sf::Color::Red),
				sf::Vertex(sf::Vector2f(qt->xOrigin + qt->width, qt->yOrigin + qt->height), sf::Color::Red),
				sf::Vertex(sf::Vector2f(qt->xOrigin + qt->width, qt->yOrigin), sf::Color::Red),
				sf::Vertex(sf::Vector2f(qt->xOrigin, qt->yOrigin), sf::Color::Red)
			};

			window.draw(vertices, 5, sf::LineStrip);
		}

		// Display
		window.display();
	}


	return 0;
}