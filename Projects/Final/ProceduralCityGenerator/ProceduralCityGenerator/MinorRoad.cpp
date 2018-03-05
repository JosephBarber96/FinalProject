#include "MinorRoad.h"

MinorRoad::MinorRoad() {}

MinorRoad::MinorRoad(int _sx, int _sy, int _ex, int _ey)
	:
	Road(_sx, _sy, _ex, _ey)
{}

MinorRoad::MinorRoad(Vec2* s, Vec2* e)
	:
	Road(s, e)
{}

MinorRoad::~MinorRoad() {}