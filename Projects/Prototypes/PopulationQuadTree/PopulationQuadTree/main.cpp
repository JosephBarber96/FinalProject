#include <SFML\Graphics.hpp>

const int winX = 500;
const int winY = 500;

int main()
{
	// Create a window
	sf::RenderWindow window(sf::VideoMode(winX, winY), "Population Data Quad Tree");

	// Read in population data

	// Create a quad tree

	// Display
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) { window.close(); }
		}
	}


	return 0;
}