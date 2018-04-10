#pragma once
#include <vector>
class RoadNode;
class Road
{
public:
	Road();
	~Road();

	static std::vector<Road*> roads;

	std::vector<RoadNode*> nodes;
};

