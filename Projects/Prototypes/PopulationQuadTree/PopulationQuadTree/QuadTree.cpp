#include "QuadTree.h"

std::vector<QuadTree*> QuadTree::Children;

QuadTree::QuadTree() {}

QuadTree::QuadTree(int newXOrigin, int newYOrigin, int newWidth, int newHeight)
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

	Children.push_back(this);

	CheckSplit();
}

QuadTree::~QuadTree() {}

bool QuadTree::CheckSplit()
{
	return false;
}

void QuadTree::Split()
{
	// Width and Height halves for all children
	int newWidth = width / 2;
	int newHeight = height / 2;

	// Top left
	// Xorigin remains the same
	// Yorigin increases
	topLeft = new QuadTree(xOrigin, yOrigin + newHeight, newWidth, newHeight);

	// Top Right
	// Xorigin increases
	// Yorigin increases
	topRight = new QuadTree(xOrigin + newWidth, yOrigin + newHeight, newWidth, newHeight);

	// Bottom Left
	// Xorigin remains the same
	// Yorigin remains the same
	bottomLeft = new QuadTree(xOrigin, yOrigin, newWidth, newHeight);

	// Bottom Right
	// Xorigin increasesd
	// Yorigin remains the same
	bottomRight = new QuadTree(xOrigin + newWidth, yOrigin, newWidth, newHeight);

}