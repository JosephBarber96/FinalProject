#include "jbPentagon.h"
#include "V2.h"

jbPentagon::jbPentagon()
	:
	jbShape()
{
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