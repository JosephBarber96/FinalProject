#include <vector>
#include <iostream>
#include <math.h>

#include <SFML\Graphics.hpp>

#include "DiamondSquare.h"


int main()
{
	int divisions = 512;
	int size = 5;
	int height = 50;
	DiamondSquare ds = DiamondSquare(divisions, size, height);
	ds.Generate();
	// ds.Print();
	ds.CreatePoints();
	ds.CalcuateBoundaryPoints();

	sf::RenderWindow window(sf::VideoMode(512, 512), "Window");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) { window.close(); }
		}

		window.clear();
		
		int numOfVerts = ds.Points().size() * ds.Points()[0].size();
		sf::VertexArray vertPoints(sf::Points, numOfVerts);
		int counter = 0;
		for (auto vec : ds.Points())
		{
			for (auto v : vec)
			{
				// Position
				vertPoints[counter].position = sf::Vector2f(v->x, v->y);

				// Color
				sf::Color shapeColor = sf::Color::White;
				int total = ds.Highest();
				float current = v->z;
				current += (abs(ds.Lowest()));
				float percentage = (current * 100) / total;
				float singlePercent = 255 / 100;
				float fillAmount = singlePercent * percentage;
				shapeColor.a = fillAmount;
				vertPoints[counter].color = shapeColor;

				counter++;
			}
		}
		window.draw(vertPoints); 

		window.display();
	}

	return 0;
}