#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <math.h>

#include "FastNoise.h"
#include <SFML\Graphics.hpp>

#include "QuadTree.h"
#include "MinimumSpanningTree.h"
#include "Node.h"
#include "MstNode.h"
#include "Edge.h"
#include "V2.h"

void makeAllValuesPositive(std::vector<std::vector<float>> &popMap, float &highestVal)
{
	// Find the lowest and highest values
	float highest = INT_MIN;
	float lowest = INT_MAX;
	for (auto vec : popMap)
	{
		for (auto val : vec)
		{
			if (val > highest) highest = val;
			if (val < lowest) lowest = val;
		}
	}

	highestVal = highest;

	// Add the absolute of the lowest onto every value
	// so it ranges from 0-highest+abs(lowest) and not -1 - 1
	float toAdd = fabsf(lowest);
	highestVal += toAdd;
	for (auto &vec : popMap)
	{
		for (auto &val : vec)
		{
			val += toAdd;
		}
	}

}

void seedNoise(FastNoise &fn)
{
	srand(time(NULL));

	fn.SetSeed(rand() % INT_MAX);
}

void fillNoise(FastNoise &fn, std::vector<std::vector<float>> &popMap, int size)
{
	for (int y = 0; y < size; y++)
	{
		popMap.push_back(std::vector<float>());
		for (int x = 0; x < size; x++)
		{
			popMap[y].push_back(fn.GetNoise(x, y));
		}
	}
}

bool drawQT = false;

int main()
{
	// Size of map
	const int size = 512;

	/* Generate noise */
	FastNoise fn;
	fn.SetNoiseType(FastNoise::ValueFractal);
	std::vector<std::vector<float>> popMap;
	seedNoise(fn);
	fn.SetFrequency(0.01); // Default 0.01
	fn.SetFractalOctaves(6); // Default 3
	fn.SetInterp(FastNoise::Interp::Quintic); // Default Quintic
	fillNoise(fn, popMap, size);

	/* Make the values positive */
	float highestVal;
	makeAllValuesPositive(popMap, highestVal);
	std::cout << "Highest value: " << highestVal << std::endl;

	/* Quad tree */
	QuadTree* qt = new QuadTree(0, 0, size, size, nullptr, popMap, size, highestVal);

	/* MST */
	MinimumSpanningTree* mst = new MinimumSpanningTree();
	for (QuadTree* quad : qt->GetTreeChildren())
	{
		mst->SpawnPoint(quad->xOrigin, quad->yOrigin, quad->xOrigin + quad->width, quad->yOrigin + quad->height);
	}
	float maxDist = size / 4;
	mst->AssignNighbours(maxDist);
	mst->CreateAllEdges();
	mst->Sort();


	sf::RenderWindow window(sf::VideoMode(size, size), "Noise population map");
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) { window.close(); }
			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
				{
					drawQT = !drawQT;
				}
			}
		}

		window.clear();

		/* Draw popmap */
		int numOfPoints = size*size;
		sf::VertexArray popPoints(sf::Points, numOfPoints);
		int counter = 0;
		int x = 0, y = 0;
		for (auto vec : popMap)
		{
			for (auto val : vec)
			{
				// Position
				popPoints[counter].position = sf::Vector2f(x, y);

				// Color
				sf::Color pointColour = sf::Color::White;
				float total = highestVal;
				float current = val;

				// How much percentage of the highest value is the current value
				float percentage = (current * 100) / total;

				// What is the alpha value of a single percent
				float singlePercentColour = 255 / 100;

				// How much must we therefore fill in this pixel for
				float fillAmount = singlePercentColour * percentage;
				
				pointColour.a = fillAmount;
				popPoints[counter].color = pointColour;

				counter++;
				x++;
			}
			y++;
			x = 0;
		}
		window.draw(popPoints);

		/* Draw Quad tree */
		if (drawQT)
		{
			for (auto quad : qt->GetHead()->GetTreeChildren())
			{
				sf::Vertex vertices[5] =
				{
					sf::Vertex(sf::Vector2f(quad->xOrigin, quad->yOrigin), sf::Color::Red),
					sf::Vertex(sf::Vector2f(quad->xOrigin, quad->yOrigin + quad->height), sf::Color::Red),
					sf::Vertex(sf::Vector2f(quad->xOrigin + quad->width, quad->yOrigin + quad->height), sf::Color::Red),
					sf::Vertex(sf::Vector2f(quad->xOrigin + quad->width, quad->yOrigin), sf::Color::Red),
					sf::Vertex(sf::Vector2f(quad->xOrigin, quad->yOrigin), sf::Color::Red)
				};

				window.draw(vertices, 5, sf::LineStrip);
			}
		}

		/* Draw MST */
		for (Edge edge : mst->GetTreeEdges())
		{
			sf::Vertex edgeVertices[2] =
			{
				sf::Vertex(sf::Vector2f(edge.start->position->x, edge.start->position->y), sf::Color::Blue),
				sf::Vertex(sf::Vector2f(edge.end->position->x, edge.end->position->y), sf::Color::Blue)
			};
			window.draw(edgeVertices, 2, sf::Lines);
		}
		

		window.display();
	}

	return 0;
}