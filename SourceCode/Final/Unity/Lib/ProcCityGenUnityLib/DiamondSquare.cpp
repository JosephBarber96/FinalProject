#include <iostream>
#include <iomanip>	// for std::fixed and std::setprecision
#include <math.h>	// for log2

#include "DiamondSquare.h"
#include "UtilRandom.h"
#include "RoadNode.h"
#include "WaterData.h"
#include "Vec3.h"

DiamondSquare::DiamondSquare(int div, float siz, float hei)
	:
	divisions(div),
	size(siz),
	height(hei)
{}

DiamondSquare::~DiamondSquare() {}

void DiamondSquare::Generate()
{
	// Vert count
	// 4 faces along = 5 vertsalong , etc;
	// its 2(n+1)
	vertCount = (divisions + 1)*(divisions + 1);
	terrainVertices.resize(vertCount);

	// Tris
	// Number of tris = divions*visions*2 for how many triangles we have
	// but because tris are calculated by 3 points its *6
	const int numOfTris = divisions*divisions * 6;
	std::vector<int> tris;
	tris.resize(numOfTris);

	// Size
	float halfSize = size*0.5f;
	float divisionSize = size / divisions;

	// Settings up the tris...
	int triOffset = 0;
	for (int i = 0; i <= divisions; i++)
	{
		for (int j = 0; j < divisions; j++)
		{
			terrainVertices[i*(divisions + 1) + j] = Vec3(-halfSize + j*divisionSize, 0.0f, halfSize - i*divisionSize);

			if (i < divisions && j < divisions)
			{
				int topLeft = i*(divisions + 1) + j;
				int bottomLeft = (i + 1)*(divisions + 1) + j;

				// Triangle verts
				tris[triOffset] = topLeft;
				tris[triOffset + 1] = topLeft + 1;
				tris[triOffset + 2] = bottomLeft + 1;

				tris[triOffset + 3] = topLeft;
				tris[triOffset + 4] = bottomLeft + 1;
				tris[triOffset + 5] = bottomLeft;

				triOffset += 6;
			}
		}
	}


	// Diamond Square...

	// First, we have to offset the four corners of the square
	terrainVertices[0].y = UtilRandom::Instance()->RandomInt(-height, height);			// top left
	terrainVertices[divisions].y = UtilRandom::Instance()->RandomInt(-height, height);	// top right
	terrainVertices[terrainVertices.size() - 1].y = UtilRandom::Instance()->RandomInt(-height, height);	// bottom right
	terrainVertices[(terrainVertices.size() - 1) - divisions].y = UtilRandom::Instance()->RandomInt(-height, height);	// bottom left

																														// Next, diamond square algorithm
	float iterations = (int)log2(divisions);
	int numSquares = 1;
	int squareSize = divisions;

	for (int i = 0; i < iterations; i++)		// Current iteration
	{
		int row = 0;
		for (int j = 0; j < numSquares; j++)
		{
			int col = 0;
			for (int k = 0; k < numSquares; k++)
			{
				DiaSqu(row, col, squareSize, height);
				col += squareSize;
			}
			row += squareSize;
		}
		numSquares *= 2;
		squareSize /= 2;
		height /= 2;
	}

}

void DiamondSquare::Print()
{
	int faces = divisions;
	int vertsPerLine = divisions + 1;

	std::cout << "Y values: " << std::endl;
	int counter = 0;
	for (auto tri : terrainVertices)
	{
		std::cout << std::fixed;
		std::cout << std::setprecision(2);
		std::cout << tri.y << '\t';

		counter++;
		if (counter == vertsPerLine)
		{
			counter = 0;
			std::cout << std::endl;
		}
	}
}

void DiamondSquare::CalcuateBoundaryPoints()
{
	// Find the highest and lowest points
	lowest = INT_MAX;
	highest = INT_MIN;
	for (auto vec : points)
	{
		for (auto v : vec)
		{
			if (v->z < lowest) lowest = v->z;
			if (v->z > highest) highest = v->z;
		}
	}
	// Convert it into positive space
	highest += abs(lowest);

	// std::cout << "Highest: " << highest << std::endl;
}

void DiamondSquare::CreatePoints()
{
	points = std::vector<std::vector<Vec3*>>();
	points.push_back(std::vector<Vec3*>());

	int vertsperLine = getDivisions() + 1;
	int x = 0;
	int y = 0;
	for (auto tri : Verts())
	{
		points[points.size() - 1].push_back(new Vec3(x, y, tri.y));

		x++;
		if (x == vertsperLine)
		{
			points.push_back(std::vector<Vec3*>());
			x = 0;
			y++;
		}
	}

	points.erase(points.end() - 1);
}

std::vector<std::vector<RoadNode*>> DiamondSquare::CreatePointsAndPassBackRoadNodes(int offsetForRoadNodes, WaterData* wd)
{
	std::vector<std::vector<RoadNode*>> roadNodes;
	roadNodes.push_back(std::vector<RoadNode*>());

	points = std::vector<std::vector<Vec3*>>();
	points.push_back(std::vector<Vec3*>());

	int vertsperLine = getDivisions() + 1;
	int xIndex = 0;
	int yIndex = 0;
	int x = 0;
	int y = 0;
	for (auto tri : Verts())
	{
		points[points.size() - 1].push_back(new Vec3(x, y, tri.y));

		// Only insert RoadNode on every Nth loop
		if (x % offsetForRoadNodes == 0 && y % offsetForRoadNodes == 0)
		{
			// High cost
			if (wd->IsWater(x, y))
			{
				roadNodes[roadNodes.size() - 1].push_back(new RoadNode(xIndex, yIndex, x, y, highest * 1.5f));
			}
			// Average cost
			else
			{
				roadNodes[roadNodes.size() - 1].push_back(new RoadNode(xIndex, yIndex, x, y, (tri.y + abs(lowest))));
			}
			xIndex++;
		}

		x++;
		if (x == vertsperLine)
		{
			points.push_back(std::vector<Vec3*>());

			// Only insert RoadNode on every Nth loop
			if (y % offsetForRoadNodes == 0)
			{
				roadNodes.push_back(std::vector<RoadNode*>());
				yIndex++;
				xIndex = 0;
			}

			x = 0;
			y++;
		}
	}

	points.erase(points.end() - 1);
	roadNodes.erase(roadNodes.end() - 1);

	return roadNodes;
}

float DiamondSquare::GetHeightAtPoint(int x, int y)
{
	return (points[x][y]->z);
}

void DiamondSquare::DiaSqu(int row, int col, int size, float heightOffset)
{
	int halfSize = (int)size*0.5f;
	int topLeft = row * (divisions + 1) + col;
	int bottomLeft = (row + size) * (divisions + 1) + col;

	int middlePoint = (int)(row + halfSize)*(divisions + 1) + (int)(col + halfSize);

	// Diamond

	terrainVertices[middlePoint].y = (terrainVertices[topLeft].y + terrainVertices[topLeft + size].y + terrainVertices[bottomLeft].y + terrainVertices[bottomLeft + size].y) * 0.25f;
	terrainVertices[middlePoint].y += UtilRandom::Instance()->RandomInt(-heightOffset, heightOffset);

	// Square

	terrainVertices[topLeft + halfSize].y = (terrainVertices[topLeft].y + terrainVertices[topLeft + size].y + terrainVertices[middlePoint].y) / 3;
	terrainVertices[topLeft + halfSize].y += UtilRandom::Instance()->RandomInt(-heightOffset, heightOffset);

	terrainVertices[middlePoint - halfSize].y = (terrainVertices[topLeft].y + terrainVertices[bottomLeft].y + terrainVertices[middlePoint].y) / 3;
	terrainVertices[middlePoint - halfSize].y += UtilRandom::Instance()->RandomInt(-heightOffset, heightOffset);

	terrainVertices[middlePoint + halfSize].y = (terrainVertices[topLeft + size].y + terrainVertices[bottomLeft + size].y + terrainVertices[middlePoint].y) / 3;
	terrainVertices[middlePoint + halfSize].y += UtilRandom::Instance()->RandomInt(-heightOffset, heightOffset);

	terrainVertices[bottomLeft + halfSize].y += (terrainVertices[bottomLeft].y + terrainVertices[bottomLeft + size].y + terrainVertices[middlePoint].y) / 3;
	terrainVertices[bottomLeft + halfSize].y += UtilRandom::Instance()->RandomInt(-heightOffset, heightOffset);
}