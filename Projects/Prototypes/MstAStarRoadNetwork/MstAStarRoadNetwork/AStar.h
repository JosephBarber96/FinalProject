#pragma once
#include <vector>
class RoadNode;
class AStar
{
public:
	AStar();
	~AStar();

	static std::vector<RoadNode*> PathFind(int startX, int startY, int endX, int endY);
	static std::vector<RoadNode*> ReconstructPath(RoadNode* goal);
};

