#include "Line.h"
#include "Vec2.h"

Line::Line() {}

Line::~Line() {}

Line::Line(Vec2* s, Vec2* e)
	:
	start(s),
	end(e)
{}

Line::Line(float sx, float sy, float ex, float ey)
	:
	start(new Vec2(sx, sy)),
	end(new Vec2(ex, ey))
{}