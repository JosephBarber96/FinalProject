#pragma once
#include <vector>
#include <SFML\Graphics.hpp>
class PopulationBox
{
public:
	PopulationBox();
	~PopulationBox();

	void FloodFill(sf::Image populationMap, int x, int y);
	bool inBounds(int x, int y);

	int getLowestX() const { return lowestX; }
	int getHighestX() const { return highestX; }
	int getLowestY() const { return lowestY; }
	int getHighestY() const { return highestY; }

private:
	void FindBoundaries(std::vector<Vec2*> points);
	int lowestX, lowestY, highestX, highestY;

};

