#include "Line.h"
#include "V2.h"

Line::Line() {}

Line::~Line() {}

Line::Line(V2* s, V2* e)
	:
	start(s),
	end(e)
{}

Line::Line(float sx, float sy, float ex, float ey)
	:
	start(new V2(sx, sy)),
	end(new V2(ex, ey))
{}