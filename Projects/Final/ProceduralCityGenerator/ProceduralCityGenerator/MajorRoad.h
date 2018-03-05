#pragma once
#include "Road.h"

class MajorRoad : public Road
{
public:
	MajorRoad();
	MajorRoad(int _sx, int _sy, int _ex, int _ey);
	MajorRoad(Vec2* s, Vec2* e);
	~MajorRoad();
};