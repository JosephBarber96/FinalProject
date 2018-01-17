#include "jbPentagon.h"
#include "V2.h"

jbPentagon::jbPentagon()
	:
	jbShape()
{
	type = Shape::pentagon;

	// Create shape
	ResetShape();
}

void jbPentagon::ResetShape()
{
	// Clear any existing points
	points.clear();

	// Bot left
	points.push_back(new V2(20, 0));

	// Mid left
	points.push_back(new V2(0, 60));

	// Top
	points.push_back(new V2(50, 100));

	// Mid right
	points.push_back(new V2(100, 60));

	// Bot right
	points.push_back(new V2(80, 0));

	// Lines
	GenerateLines();
}

jbPentagon::~jbPentagon() {}

bool jbPentagon::Equal_To(jbShape* shape)
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