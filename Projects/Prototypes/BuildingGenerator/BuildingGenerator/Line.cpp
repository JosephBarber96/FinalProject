#include "Line.h"
#include "V2.h"

Line::Line() {}

Line::Line(V2* _start, V2* _end)
	:
	start(_start),
	end(_end)
{}

Line::Line(float sX, float sY, float eX, float eY)
	:
	start(new V2(sX, sY)),
	end(new V2(eX, eY))
{}

Line::~Line() {}