#pragma once
#include <vector>
class Vec3;
class MidpointDisplacement
{
public:
	MidpointDisplacement();
	~MidpointDisplacement();

	std::vector<Vec3*> points;

};

