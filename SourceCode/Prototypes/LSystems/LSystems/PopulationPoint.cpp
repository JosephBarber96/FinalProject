#include "PopulationPoint.h"

PopulationPoint::PopulationPoint(float newX, float newY)
	:
	Vec2(newX, newY),
	marked(false)
{}

PopulationPoint::~PopulationPoint() {}