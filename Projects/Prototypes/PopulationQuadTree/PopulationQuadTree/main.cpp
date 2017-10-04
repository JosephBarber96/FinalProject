#include <array>
#include <stdlib.h>
#include <ctime>

#include <SFML\Graphics.hpp>

#include "QuadTree.h"

//const int winX = 500;
//const int winY = 500;

int main()
{
	// Read in population data
	sf::Image populationMap;
	if (!populationMap.loadFromFile("PopulationMap.bmp"))
	{
		return -1;
	}

	/////////////////////
	// TEST 
	/////////////////////

	// Randomly make population data for testing now
	srand(time(NULL));

	//sf::Image populationMap;
	//populationMap.create(100, 100, sf::Color::White);

	for (int i = 0; i < 100; i++)
	{
		int x = rand() % 100;
		int y = rand() % 100;

		populationMap.setPixel(x, y, sf::Color::Black);
	}

	populationMap.saveToFile("SavedImage.bmp");

	/////////////////////
	// END TEST
	/////////////////////

	// Create a sprite to display this image;
	sf::Texture populationTexture;
	populationTexture.update(populationMap);
	populationTexture.setSmooth(true);

	sf::Sprite populationSprite;
	populationSprite.setTexture(populationTexture);
	populationSprite.setPosition(0, 0);

	// Create a window
	sf::RenderWindow window(sf::VideoMode(500, 500), "Population Data Quad Tree");

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