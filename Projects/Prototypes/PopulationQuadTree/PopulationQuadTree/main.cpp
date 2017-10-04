#include <array>
#include <stdlib.h>
#include <ctime>
#include <string>

#include <SFML\Graphics.hpp>

#include "QuadTree.h"

int winX;
int winY;

int main()
{
	std::string filename = "PopulationMap.jpg";

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



	// Create a quad tree
	// QuadTree* qt = new QuadTree(200, 200, 200, 200);

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
				sf::Vertex(sf::Vector2f(qt->xOrigin, qt->yOrigin)),
				sf::Vertex(sf::Vector2f(qt->xOrigin, qt->yOrigin + qt->height)),
				sf::Vertex(sf::Vector2f(qt->xOrigin + qt->width, qt->yOrigin + qt->height)),
				sf::Vertex(sf::Vector2f(qt->xOrigin + qt->width, qt->yOrigin)),
				sf::Vertex(sf::Vector2f(qt->xOrigin, qt->yOrigin))
			};

			window.draw(vertices, 5, sf::LineStrip);
		}

		// Display
		window.display();
	}


	return 0;
}