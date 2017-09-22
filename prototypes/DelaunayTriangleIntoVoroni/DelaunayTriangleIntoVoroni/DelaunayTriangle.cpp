#include "DelaunayTriangle.h"
#include "Point.h"
#include "V2.h"
#include "Utility.h"
#include "Box.h"
#include <iostream>
DelaunayTriangle::DelaunayTriangle() {}
DelaunayTriangle::~DelaunayTriangle() {}

void DelaunayTriangle::Generate(int size)
{
	int columns = 3;
	int rows = 3;

	// Split the screen into segments
	for (int x = 0; x < columns; x++)
	{
		for (int y = 0; y < rows; y++)
		{
			int minx = 0 + ((size / columns) * x);
			int maxx = minx + size / columns;

			int miny = 0 + ((size / rows) * y);
			int maxy = miny + size / rows;

			std::cout << "Segment: (" << minx << ", " << maxx << ", " << miny << ", " << maxy << ")" << std::endl;

			screenSegments.push_back(new Box(minx, maxx, miny, maxy));
		}
	}

	// Generate points
	int numberOfPoints = (size / 10);
	int pointsPerSegment = 1; //int(numberOfPoints / screenSegments.size());

	for each (Box segment in screenSegments)
	{
		for (auto i = 0; i < pointsPerSegment; i++)
		{
			int x = rand() % (size / columns);
			x += segment.minX;
			int y = rand() % (size / rows);
			y += segment.minY;

			std::cout << "Creating point at: " << x << ", " << y << std::endl;

			points.push_back(new Point(x, y));
		}
	}

	//for (auto i = 0; i < numberOfPoints; i++)
	//{
	//	int x = rand() % size;
	//	int y = rand() % size;

	//	points.push_back(new Point(x, y));
	//}

	// Minimum distance to be considered neighbouring points
	int minNeighbourDist = float((screenSegments[0].maxX) * 1.5); //size / 4;

	// Assign neighbours
	// For each point
	for each (Point* point in points)
	{
		int neighboursAdded = 0;
		float lowestDist = FLT_MAX;
		Point* closingNeighbour = nullptr;

		// Check if every other point is close enough
		for each (Point* possibleNeighbour in points)
		{
			// Skip itself
			if (point->position->x == possibleNeighbour->position->x &&
				point->position->y == possibleNeighbour->position->y)
			{
				continue;
			}

			float dist = Utility::DistanceBetween(*point->position, *possibleNeighbour->position);

			if (dist < minNeighbourDist)
			{
				neighboursAdded == 1;
				std::cout << " Distance between neighbours = " << Utility::DistanceBetween(*point->position, *possibleNeighbour->position) << std::endl;
				point->AddNeighbour(possibleNeighbour);
			}
			else if (dist<lowestDist)
			{
				lowestDist = dist;
				closingNeighbour = possibleNeighbour;
			}
		}

		if (neighboursAdded < 2)
		{
			point->AddNeighbour(closingNeighbour);
		}
	}
}
