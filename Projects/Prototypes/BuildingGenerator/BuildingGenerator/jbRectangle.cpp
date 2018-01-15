#include "jbRectangle.h"
#include "V2.h"
#include "UtilRandom.h"

jbRectangle::jbRectangle()
	:
	jbShape()
{
	// Bottom left
	points.push_back(new V2(0, 0));

	int wide = UtilRandom::Instance()->Random(0, 1);

	// Tall
	if (wide == 0)
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