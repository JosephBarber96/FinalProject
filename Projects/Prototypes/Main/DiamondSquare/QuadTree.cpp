#include "QuadTree.h"
#include "WaterData.h"

QuadTree::QuadTree(int _x, int _y, int _wid, int _hei, QuadTree* _par, std::vector<std::vector<float>> popMap, WaterData &waterData, int popMapSize, float highest)
	:
	xOrigin(_x),
	yOrigin(_y),
	width(_wid),
	height(_hei),
	parent(_par)
{

	// Check population
	if (width > 128) //256, 512
	{
		population = QuadPopulation::low;
	}
	else if (width >= 64) // 64, 128
	{
		population = QuadPopulation::med;
	}
	else // 16, 32, 64
	{
		population = QuadPopulation::high;
	}

	if (CheckSplit(popMap, waterData, popMapSize, highest))
	{
		Split(popMap, waterData, popMapSize, highest);
	}
	else
	{
		AddChildToHeadOfTree(this);
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

/* Private */

bool QuadTree::CheckSplit(std::vector<std::vector<float>> popMap, WaterData &waterData, int size, float highest)
{
	// First, check that there is land
	if (!waterData.SectionContainsLand(xOrigin, width, yOrigin, height)) return false;

	float tenPercent = highest / 10;

	float thresholds[6] =
	{
		tenPercent,			//10%
		tenPercent * 4,		//40%
		tenPercent * 6.75f,	//67.5%
		tenPercent * 8,		//80%
		tenPercent * 9,		//90%
		tenPercent * 9.5f	//95%
	};

	indexer = 0;
	int counter = 0;
	for (auto val : sizes)
	{
		if (width == sizes[counter]) indexer = counter;

		counter++;
	}

	// If minimum width already reached
	if (indexer == 5) return false;

	// See if there is a high enough population point within the quad
	for (int y = yOrigin; y < (yOrigin + height); y++)
	{
		for (int x = xOrigin; x < (xOrigin + width); x++)
		{
			if (y >= size || x >= size) continue;

			if (popMap[y][x] >= thresholds[indexer])
			{
				return true;
			}
		}
	}

	return false;
}

void QuadTree::Split(std::vector<std::vector<float>> popMap, WaterData &waterData, int size, float highest)
{
	// Width and Height halves for all children
	int newWidth = width / 2;
	int newHeight = height / 2;

	// Top left
	// Xorigin remains the same
	// Yorigin increases
	topLeft = new QuadTree(xOrigin, yOrigin + newHeight, newWidth, newHeight, this, popMap, waterData, size, highest);

	// Top Right
	// Xorigin increases
	// Yorigin increases
	topRight = new QuadTree(xOrigin + newWidth, yOrigin + newHeight, newWidth, newHeight, this, popMap, waterData, size, highest);

	// Bottom Left
	// Xorigin remains the same
	// Yorigin remains the same
	bottomLeft = new QuadTree(xOrigin, yOrigin, newWidth, newHeight, this, popMap, waterData, size, highest);

	// Bottom Right
	// Xorigin increasesd
	// Yorigin remains the same
	bottomRight = new QuadTree(xOrigin + newWidth, yOrigin, newWidth, newHeight, this, popMap, waterData, size, highest);
}