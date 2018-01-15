#include "jbHexagon.h"
#include "V2.h"

jbHexagon::jbHexagon()
	:
	jbShape()
{
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