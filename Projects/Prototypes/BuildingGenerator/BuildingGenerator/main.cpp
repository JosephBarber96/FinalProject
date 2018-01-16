#include <vector>
#include <iostream>

#include <SFML\Graphics.hpp>

#include "jbShape.h"
#include "V2.h"
#include "Line.h"
#include "FloorPlan.h"
#include "BoundingBox.h"


#include "Utility.h"

int main()
{
	FloorPlan fp;
	fp.SetBoundingBox(100, 100, 300, 300);
	fp.GenerateShapes(8);
	fp.GeneratePerimeter();

	/* Create the window */
	sf::RenderWindow window(sf::VideoMode(500, 500), "Building generation");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) { window.close(); }
		}

		window.clear();

		//fp.bb->DrawSelf(&window);
		for (auto shape : fp.shapes)
		{
			shape->DrawSelf(&window);
		}
		fp.DrawPerimeter(&window);

		window.display();
	}


	return 0;
}