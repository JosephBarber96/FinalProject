#pragma once
#include <vector>
class RoadNode;
namespace Pathfinding
{
	std::vector<RoadNode*> PathFind(std::vector<std::vector<RoadNode*>> grid, int startX, int startY, int endX, int endY, int offsetPerRoadNode);
	std::vector<RoadNode*> ReconstructPath(RoadNode* goal);
}