#include "jbSquare.h"
#include "V2.h"

jbSquare::jbSquare()
	:
	jbShape()
{
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