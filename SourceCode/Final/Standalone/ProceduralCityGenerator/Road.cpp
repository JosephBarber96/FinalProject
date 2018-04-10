#include "Road.h"
#include "Vec2.h"

Road::Road() {}

Road::Road(int _sx, int _sy, int _ex, int _ey)
	:
	startPos(new Vec2(_sx, _sy)),
	endPos(new Vec2(_ex, _ey))
{}

Road::Road(Vec2* s, Vec2* e)
	:
	startPos(s),
	endPos(e)
{}

Road::~Road() {}

bool Road::Equals(int sx, int sy, int ex, int ey)
{
	return (startPos->x == sx && startPos->y == sy && endPos->x == ex && endPos->y == ey);
}