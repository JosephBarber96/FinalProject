#include "jbHexagon.h"
#include "V2.h"

jbHexagon::jbHexagon()
	:
	jbShape()
{
	// Create shape
	ResetShape();
}

void jbHexagon::ResetShape()
{
	// Clear any existing points
	points.clear();

	// Bottom left
	points.push_back(new V2(0, 20));

	// Top left
	points.push_back(new V2(0, 80));

	// Top
	points.push_back(new V2(50, 100));

	// Top right
	points.push_back(new V2(100, 80));

	// Bottom right
	points.push_back(new V2(100, 20));

	// Bottom
	points.push_back(new V2(50, 0));

	// Lines
	GenerateLines();
}

jbHexagon::~jbHexagon() {}

bool jbHexagon::Equal_To(jbShape* shape)
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