#pragma once
#include <vector>
class RoadNode;
class Road
{
public:
	Road();
	~Road();

	std::vector<RoadNode*> nodes;
};