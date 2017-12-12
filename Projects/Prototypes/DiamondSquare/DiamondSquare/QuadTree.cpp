#include "QuadTree.h"

QuadTree::QuadTree(QuadTree* newParent, int newXOrigin, int newYOrigin, int newWidth, int newHeight, sf::Image populationMap)
	:
	parent(newParent),
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
		GetHead()->AddChild(this);
	}
}

QuadTree::~QuadTree() {}

QuadTree* QuadTree::GetHead()
{
	if (parent != nullptr)
	{
		return parent->GetHead();
	}
	return this;
}

bool QuadTree::CheckSplit(sf::Image populationMap)
{
	int counter = 0;
	std::map<int, int> colorMap;

	// Loop through each pixel of the aray, output the pixel data
	for (int y = yOrigin; y < (yOrigin + height); y += 2)
	{
		for (int x = xOrigin; x < (xOrigin + width); x += 2)
		{
			auto color = populationMap.getPixel(x, y).toInteger();

			// Int32 colours
			// -1061109505 = light grey
			// -2139062017 = dark grey
			// 255 = black
			if (color == -1061109505
				&& width > populationMap.getSize().x / 8)
			{
				return true;
			}
			else if (color == -2139062017
				&& width > populationMap.getSize().x / 12)
			{
				return true;
			}
			else if (color == 255)
			{
				// Don't split if the quad is already small enough
				if (width > populationMap.getSize().x / 17)
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
	topLeft = new QuadTree(this, xOrigin, yOrigin + newHeight, newWidth, newHeight, populationMap);

	// Top Right
	// Xorigin increases
	// Yorigin increases
	topRight = new QuadTree(this, xOrigin + newWidth, yOrigin + newHeight, newWidth, newHeight, populationMap);

	// Bottom Left
	// Xorigin remains the same
	// Yorigin remains the same
	bottomLeft = new QuadTree(this, xOrigin, yOrigin, newWidth, newHeight, populationMap);

	// Bottom Right
	// Xorigin increasesd
	// Yorigin remains the same
	bottomRight = new QuadTree(this, xOrigin + newWidth, yOrigin, newWidth, newHeight, populationMap);

}