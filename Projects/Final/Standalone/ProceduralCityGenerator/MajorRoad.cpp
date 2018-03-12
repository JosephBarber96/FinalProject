#include "MajorRoad.h"
#include "Vec2.h"

MajorRoad::MajorRoad() {}

MajorRoad::MajorRoad(int _sx, int _sy, int _ex, int _ey)
	:
	Road(_sx, _sy, _ex, _ey)
{}

MajorRoad::MajorRoad(Vec2* s, Vec2* e)
	:
	Road(s, e)
{}

MajorRoad::~MajorRoad() {}