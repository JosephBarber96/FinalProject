#pragma once
#include <SFML\Graphics.hpp>
class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(int minx, int miny, int maxx, int maxy);
	~BoundingBox();

	void DrawSelf(sf::RenderWindow* window);

	int minX, minY, maxX, maxY;
};

