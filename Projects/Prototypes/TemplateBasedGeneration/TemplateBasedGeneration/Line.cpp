#include "Line.h"
#include "Vec2.h"

Line::Line(Vec2* newStart, Vec2* newEnd)
	:
	start(newStart),
	end(newEnd)
{}

Line::Line(float startX, float startY, float endX, float endY)
	:
	start(new Vec2(startX, startY)),
	end(new Vec2(endX, endY))
{}

Line::~Line() {}

bool Line::operator==(Line b)
{
	return 
		(this->start == b.start 
		&&
			this->end == b.end);
}