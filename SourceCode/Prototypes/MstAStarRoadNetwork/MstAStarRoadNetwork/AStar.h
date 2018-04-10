#pragma once
#include <vector>
class RoadNode;
namespace AStar
{
	std::vector<RoadNode*> PathFind(int startX, int startY, int endX, int endY);
	std::vector<RoadNode*> ReconstructPath(RoadNode* goal);
}
