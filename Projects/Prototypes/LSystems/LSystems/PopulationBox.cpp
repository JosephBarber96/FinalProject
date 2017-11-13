#include <queue>
#include <vector>

#include "PopulationBox.h"
#include "vec2.h"

PopulationBox::PopulationBox() {}

PopulationBox::~PopulationBox() {}

std::vector<Vec2*> getNeighbours(Vec2* current, int maxX, int maxY)
{
	std::vector<Vec2*> neighbours;

	// Left (-1 x)
	if (current->getX() > 0) neighbours.push_back(new Vec2(current->getX() - 1, current->getY()));

	// Up (-1 y)
	if (current->getY() > 0) neighbours.push_back(new Vec2(current->getX(), current->getY() - 1));

	// Right (+1 x)
	if (current->getX() < maxX) neighbours.push_back(new Vec2(current->getX() + 1, current->getY()));

	// Down (+1 y)
	if (current->getY() < maxY) neighbours.push_back(new Vec2(current->getX(), current->getY() + 1));
}

void PopulationBox::FloodFill(sf::Image popMap, int startX, int startY)
{
	std::vector<Vec2*> vecsInPopulation;

	std::queue<Vec2*> frontier;

	std::vector<Vec2*> nextGen;

	Vec2* start = new Vec2(startX, startY);
	frontier.push(start);

	bool quit = false;
	while (!quit)
	{
		// For each item in the frontier...
		while (!frontier.empty())
		{
			Vec2* current = frontier.front();
			frontier.pop();

			// Get each of the items neighbours...
			std::vector<Vec2*> neighbours = getNeighbours(current, popMap.getSize().x, popMap.getSize().y);

			// For each neighbour...
			for (auto vec : neighbours)
			{
				// If this neighour is a black pixel...
				if (popMap.getPixel(vec->getX(), vec->getY()).toInteger() == 255)
				{
					// Push it into nextGen
					nextGen.push_back(vec);
				}
			}
		}

		// If nextGen is empty, we have nothing else to check, so quit...
		if (nextGen.size() == 0) quit = true;

		// Else, frontier becomes nextGen
		for (auto vec : nextGen) frontier.push(vec);

		// Empty nextGen
		while (nextGen.size() != 0) nextGen.erase(nextGen.begin());
	}

	FindBoundaries(vecsInPopulation);
}

bool PopulationBox::inBounds(int x, int y)
{
	bool inX = (x > lowestX && x < highestX);
	bool inY = (y > lowestY && y < highestY);

	return (inX && inY);
}

void PopulationBox::FindBoundaries(std::vector<Vec2*> points)
{
	bool lowestXFound = false, lowestYFound = false, highestXFound = false, highestYFound = false;

	for (auto vec : points)
	{
		if (!lowestXFound) { lowestXFound = true; lowestX = vec->getX(); }
		else if (vec->getX() < lowestX) { lowestX = vec->getX(); }
		if (!lowestYFound) { lowestYFound = true; lowestY = vec->getY(); }
		else if (vec->getY() < lowestY) { lowestY = vec->getY(); }
		if (!highestXFound) { highestXFound = true; highestX = vec->getX(); }
		else if (vec->getX() > highestX) { highestX = vec->getX(); }
		if (!highestYFound) { highestYFound = true; highestY = vec->getY(); }
		else if (vec->getY() > highestY) { highestY = vec->getY(); }


	}


}