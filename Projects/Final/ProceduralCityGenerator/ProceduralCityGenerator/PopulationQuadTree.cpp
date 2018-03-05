#include "PopulationQuadTree.h"
#include "PopulationMap.h"
#include "WaterData.h"

PopulationQuadTree::PopulationQuadTree(
	int newX, int newY, int newWid, int newHei,
	QuadTree* newParent,
	PopulationMap* popMap,
	WaterData* waterData,
	int popMapSize)
	:
	QuadTree(newX, newY, newWid, newHei, newParent)
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

	// See if we need to split
	if (CheckSplit(popMap, waterData, popMapSize))
	{
		Split(popMap, waterData, popMapSize);
	}
	else
	{
		AddChildToHeadOfTree(this);
	}
}

PopulationQuadTree::~PopulationQuadTree() {}

bool PopulationQuadTree::CheckSplit(PopulationMap* popMap, WaterData* waterData, int size)
{
	// First, check that there is land
	if (!waterData->SectionContainsLand(xOrigin, width, yOrigin, height)) return false;

	float tenPercent = popMap->GetHighestValue() / 10;

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

			if (popMap->GetValueAtPoint(x, y) >= thresholds[indexer])
			{
				return true;
			}
		}
	}

	return false;
}

void PopulationQuadTree::Split(PopulationMap* popMap, WaterData* waterData, int size)
{
	// Width and Height halves for all children
	int newWidth = width / 2;
	int newHeight = height / 2;

	// Top left
	// Xorigin remains the same
	// Yorigin increases
	topLeft = new PopulationQuadTree(xOrigin, yOrigin + newHeight, newWidth, newHeight, this, popMap, waterData, size);

	// Top Right
	// Xorigin increases
	// Yorigin increases
	topRight = new PopulationQuadTree(xOrigin + newWidth, yOrigin + newHeight, newWidth, newHeight, this, popMap, waterData, size);

	// Bottom Left
	// Xorigin remains the same
	// Yorigin remains the same
	bottomLeft = new PopulationQuadTree(xOrigin, yOrigin, newWidth, newHeight, this, popMap, waterData, size);

	// Bottom Right
	// Xorigin increasesd
	// Yorigin remains the same
	bottomRight = new PopulationQuadTree(xOrigin + newWidth, yOrigin, newWidth, newHeight, this, popMap, waterData, size);
}