#include "Line.h"
#include "Vec2.h"

Line::Line(Vec2* newStart, Vec2* newEnd)
	:
	start(newStart),
	end(newEnd)
{}

Line::~Line() {}
