#include "DelaunayTriangle.h"
#include "Point.h"
#include "V2.h"
#include "Utility.h"
#include "ScreenSegment.h"
#include <iostream>
#include <random>
DelaunayTriangle::DelaunayTriangle() {}
DelaunayTriangle::~DelaunayTriangle() {}

void DelaunayTriangle::GenerateDelaunay(const int screenMin, const int screenMax, const int columns, const int rows, bool withDiagonalSplit)
{
	screenSegments.resize(rows);

	int screenSize = screenMax - screenMin;

	int segWidth = screenSize / columns;
	int segHeight = screenSize / rows;


	// Split the screen into segments
	for (int y = 0; y < rows; y++)
	{
		int columnCounter = (y % 2 == 0) ? columns : (columns + 1);

		for (int x = 0; x < columnCounter; x++)
		{
			int minY = screenMin + ((screenSize / rows) * y);
			int maxY = minY + (screenSize / rows);

			int minX = screenMin + ((screenSize / columns) * x);
			if (y % 2 == 0) { minX += segWidth / 2; }
			int maxX = minX + (screenSize / columns);

			std::cout << "Segment: (" << minX << ", " << maxX << ", " << minY << ", " << maxY << ")" << std::endl;
			screenSegments[y].push_back(new ScreenSegment(minX, maxX, minY, maxY));
		}
	}

	// Generate points
	int pointsPerSegment = 1; //int(numberOfPoints / screenSegments.size());

	// Calculating the minimum and maximum boundaries a point can be placed within a screen segment
	int floorX = segWidth / 5;
	int ceilingX = segWidth - floorX;
	int rangeX = (ceilingX - floorX) + 1;

	int floorY = segHeight / 5;
	int ceilingY = segHeight - floorY;
	int rangeY = (ceilingY - floorY) + 1;

	// Loops through screen segments
	for (int y = 0; y < rows; y++)
	{
		int columnCounter = (y % 2 == 0) ? columns : (columns + 1);

		for (int x = 0; x < columnCounter; x++)
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
	for (int y = 0; y < rows; y++)
	{
		// Longer rows - Only check left and right
		if (y % 2 == 1)
		{
			for (int x = 0; x < columns + 1; x++)
			{
				// Right
				if (x < columns - 1)
				{
					screenSegments[y][x]->point->AddNeighbour(screenSegments[y][x + 1]->point);
				}

				// Left
				if (x > 0)
				{
					screenSegments[y][x]->point->AddNeighbour(screenSegments[y][x - 1]->point);
				}
			}
		}
		// Shorter rows - check all directions
		else
		{
			for (int x = 0; x < columns; x++)
			{
				// Up left
				if (y > 0)
				{
					screenSegments[y][x]->point->AddNeighbour(screenSegments[y - 1][x]->point);
				}
				// Up right
				if (y > 0 && x < rows - 1)
				{
					screenSegments[y][x]->point->AddNeighbour(screenSegments[y - 1][x + 1]->point);
				}
				// Right
				if (x < columns - 1)
				{
					screenSegments[y][x]->point->AddNeighbour(screenSegments[y][x + 1]->point);
				}
				// Down left
				if (y < rows - 1)
				{
					screenSegments[y][x]->point->AddNeighbour(screenSegments[y + 1][x]->point);
				}
				// Down right
				if (y < rows - 1 && x < columns - 1)
				{
					screenSegments[y][x]->point->AddNeighbour(screenSegments[y + 1][x + 1]->point);
				}
				// Left
				if (x > 0)
				{
					screenSegments[y][x]->point->AddNeighbour(screenSegments[y][x - 1]->point);
				}
			}
		}
	}
}
