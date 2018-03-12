#pragma once

#include "QuadTree.h"

enum QuadPopulation
{
	low,
	med,
	high
};

class PopulationMap;
class WaterData;
class PopulationQuadTree : public QuadTree
{
public:
	PopulationQuadTree(
		int newX, int newY, int newWid, int newHei,
		QuadTree* newParent,
		PopulationMap* popMap,
		WaterData* waterData,
		int popMapSize);

	~PopulationQuadTree();

	int Indexer() const { return indexer; }
	QuadPopulation PopulationDensity() const { return population; }

private:
	bool CheckSplit(PopulationMap* popMap, WaterData* waterData, int size);
	void Split(PopulationMap* popMap, WaterData* waterData, int size);

	QuadPopulation population;
	int indexer;
	const int sizes[6] =
	{
		512,
		256,
		128,
		64,
		32,
		16
	};
};