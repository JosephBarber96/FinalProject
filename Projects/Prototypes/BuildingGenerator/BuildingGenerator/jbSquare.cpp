#include "jbSquare.h"
#include "V2.h"

jbSquare::jbSquare()
	:
	jbShape()
{
	type = Shape::square;

	// Generate shape
	ResetShape();
}

void jbSquare::ResetShape()
{
	// Clear any existing points
	points.clear();

	// Bottom left
	points.push_back(new V2(0, 0));

	// Top left
	points.push_back(new V2(0, 100));

	// Top right
	points.push_back(new V2(100, 100));

	// Bottom right
	points.push_back(new V2(100, 0));

	// Lines
	GenerateLines();
}

jbSquare::~jbSquare() {}

bool jbSquare::Equal_To(jbShape* shape)
{
	// We are not equal if we have a different number of vertices
	if (points.size() != shape->points.size())
	{
		return false;
	}

	// Check every point
	for (int i = 0; i < points.size(); i++)
	{
		// If any point differs, we are not the same
		if (*points[i] != *shape->points[i])
		{
			return false;
		}
	}

	// Otherwise, we are equal
	return true;
}