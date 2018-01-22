#pragma once
#include <SFML\Graphics.hpp>
class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(int _xOri, int _yOri, int _wid, int _hei);
	~BoundingBox();

	void DrawSelf(sf::RenderWindow* window);
	bool inBounds(int x, int y);

	int xOrigin, yOrigin, width, height;
};