#include "Road.h"
#include "Utility.h"
#include "Vec2.h"

Road::Road() {}

Road::Road(Vec2* newStart, Vec2* newEnd)
	:
	start(newStart),
	end(newEnd)
{}

Road::Road(float startX, float startY, float endX, float endY)
	:
	start(new Vec2(startX, startY)),
	end(new Vec2(endX, endY))
{}

Road::~Road() {}

bool Road::operator==(Road b)
{
	return
		(this->start == b.start
			&&
			this->end == b.end);
}