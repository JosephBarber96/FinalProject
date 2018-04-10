#include "jbRectangle.h"
#include "V2.h"
#include "UtilRandom.h"

jbRectangle::jbRectangle()
	:
	jbShape()
{
	orientation = UtilRandom::Instance()->Random(0, 1);
	type = Shape::rectangle;

	// Generate shape
	ResetShape();
}

void jbRectangle::ResetShape()
{
	// Clear any existing points
	points.clear();

	// Bottom left
	points.push_back(new V2(0, 0));

	// Tall
	if (orientation == 0)
	{
		// Top left
		points.push_back(new V2(0, 150));

		// Top right
		points.push_back(new V2(100, 150));

		// Bottom right
		points.push_back(new V2(100, 0));
	}
	// Wide
	else
	{
		// Top left
		points.push_back(new V2(0, 100));

		// Top right
		points.push_back(new V2(150, 100));

		// Bottom right
		points.push_back(new V2(150, 0));
	}

	// Lines
	GenerateLines();
}

jbRectangle::~jbRectangle() {}

bool jbRectangle::Equal_To(jbShape* shape)
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