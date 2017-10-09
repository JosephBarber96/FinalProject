#include "QuadTree.h"

std::vector<QuadTree*> QuadTree::Children;

QuadTree::QuadTree() {}

QuadTree::QuadTree(int newXOrigin, int newYOrigin, int newWidth, int newHeight, sf::Image populationMap)
	:
	xOrigin(newXOrigin),
	yOrigin(newYOrigin),
	width(newWidth),
	height(newHeight)
{
	topLeft = nullptr;
	topRight = nullptr;
	bottomLeft = nullptr;
	bottomRight = nullptr;

	if (CheckSplit(populationMap))
	{
		Split(populationMap);
	}
	else
	{
		Children.push_back(this);
	}
}

QuadTree::~QuadTree() {}

bool QuadTree::CheckSplit(sf::Image populationMap)
{
	int counter = 0;
	std::map<int, int> colorMap;

	// Loop through each pixel of the aray, output the pixel data
	for (int y = yOrigin; y < (yOrigin + height); y++)
	{
		for (int x = xOrigin; x < (xOrigin + width); x++)
		{
			auto color = populationMap.getPixel(x, y).toInteger();

			// 255 is the int32 value for black
			if (color == 255)
			{
				// Don't split if the quad is already small enough
				if (width > populationMap.getSize().x / 8)
				{
					return true;
				}
				return false;
			}
		}
	}

	// If we get through it without finding black, we don't split
	return false;
}

void QuadTree::Split(sf::Image populationMap)
{
	// Width and Height halves for all children
	int newWidth = width / 2;
	int newHeight = height / 2;

	// Top left
	// Xorigin remains the same
	// Yorigin increases
	topLeft = new QuadTree(xOrigin, yOrigin + newHeight, newWidth, newHeight, populationMap);

	// Top Right
	// Xorigin increases
	// Yorigin increases
	topRight = new QuadTree(xOrigin + newWidth, yOrigin + newHeight, newWidth, newHeight, populationMap);

	// Bottom Left
	// Xorigin remains the same
	// Yorigin remains the same
	bottomLeft = new QuadTree(xOrigin, yOrigin, newWidth, newHeight, populationMap);

	// Bottom Right
	// Xorigin increasesd
	// Yorigin remains the same
	bottomRight = new QuadTree(xOrigin + newWidth, yOrigin, newWidth, newHeight, populationMap);

}