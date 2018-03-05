#pragma once
#include "Road.h"

class MinorRoad : public Road
{
public:
	MinorRoad();
	MinorRoad(int _sx, int _sy, int _ex, int _ey);
	MinorRoad(Vec2* s, Vec2* e);
	~MinorRoad();
};