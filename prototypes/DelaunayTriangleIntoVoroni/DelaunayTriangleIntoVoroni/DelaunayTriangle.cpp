#include "DelaunayTriangle.h"
#include "Point.h"
#include "V2.h"
#include "Utility.h"
#include "ScreenSegment.h"
#include <iostream>
#include <random>
DelaunayTriangle::DelaunayTriangle() {}
DelaunayTriangle::~DelaunayTriangle() {}

void DelaunayTriangle::GenerateDelaunay(const int screenSize, const int columns_, const int rows_, bool withDiagonalSplit)
{
	columns = columns_;
	rows = rows_;

	screenSegments.resize(columns_);

	// Split the screen into segments
	for (int y = 0; y < columns; y++)
	{
		for (int x = 0; x < rows; x++)
		{
			int minx = 0 + ((screenSize / columns) * x);
			int maxx = minx + screenSize / columns;

			int miny = 0 + ((screenSize / rows) * y);
			int maxy = miny + screenSize / rows;

			std::cout << "Segment: (" << minx << ", " << maxx << ", " << miny << ", " << maxy << ")" << std::endl;

			screenSegments[y].push_back(new ScreenSegment(minx, maxx, miny, maxy));
		}
	}

	// Generate points
	int pointsPerSegment = 1; //int(numberOfPoints / screenSegments.size());

	// Calculating the minimum and maximum boundaries a point can be placed within a screen segment
	int segWidth = screenSize / columns;
	int segHeight = screenSize / rows;

	int floorX = segWidth / 5;
	int ceilingX = segWidth - floorX;
	int rangeX = (ceilingX - floorX) + 1;

	int floorY = segHeight / 5;
	int ceilingY = segHeight - floorY;
	int rangeY = (ceilingY - floorY) + 1;

	// Loops through screen segments
	for (int y = 0; y < columns; y++)
	{
		for (int x = 0; x < rows; x++)
		{
			int posX = floorX + rand() % rangeX;
			posX += screenSegments[y][x]->minX;

			int posY = floorY + rand() % rangeY;
			posY += screenSegments[y][x]->minY;

			std::cout << "Creating point at: " << x << ", " << y << std::endl;
			screenSegments[y][x]->point = new Point(posX, posY);
		}
	}

	// Assign neighbours
	// Loops through screen segments
	for (int y = 0; y < columns; y++)
	{
		for (int x = 0; x < rows; x++)
		{
			// Up
			if (y > 0)
			{
				screenSegments[y][x]->point->AddNeighbour(screenSegments[y - 1][x]->point);
			}
			// Right
			if (x < rows -1)
			{
				screenSegments[y][x]->point->AddNeighbour(screenSegments[y][x + 1]->point);
			}
			// Down
			if (y < columns -1)
			{
				screenSegments[y][x]->point->AddNeighbour(screenSegments[y + 1][x]->point);
			}
			// Left
			if (x > 0)
			{
				screenSegments[y][x]->point->AddNeighbour(screenSegments[y][x - 1]->point);
			}

			// Diagonal split
			if (withDiagonalSplit)
			{
				// Up-right
				if (y > 0 && x < rows - 1)
				{
					screenSegments[y][x]->point->AddNeighbour(screenSegments[y - 1][x + 1]->point);
				}
			}
		}
	}
}
