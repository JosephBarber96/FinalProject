#include "jbTriangle.h"
#include "V2.h"

jbTriangle::jbTriangle()
	:
	jbShape()
{
	// Bottom left
	points.push_back(new V2(0, 0));

	// Top middle
	points.push_back(new V2(50, 100));

	// Bottom right
	points.push_back(new V2(100, 0));

	// Lines
	GenerateLines();
}

jbTriangle::~jbTriangle() {}