#pragma once

#include <vector>

class WaterData;
class MstNode;
class MstEdge;
class MinimumSpanningTree
{
public:
	MinimumSpanningTree();
	~MinimumSpanningTree();

	void SpawnPoint(WaterData* wd, int offset, int minX, int minY, int maxX, int maxY);
	void AssignNeighbours(float maxDistance);
	void CreateEdges();
	void Sort();

	bool TreeEdgeContainsNode(MstNode* node);
	std::vector<MstNode*> GetNodes() const { return nodes; }
	std::vector<MstEdge*> GetAllEdges() const { return allEdges; }
	std::vector<MstEdge*> GetTreeEdges() const { return treeEdges; }

private:
	void AddNode(MstNode* newNode);
	void AddPossibleEdge(MstEdge* newEdge);

	int nodeCount = 0;
	std::vector<MstNode*> nodes;
	std::vector<MstEdge*> allEdges;
	std::vector<MstEdge*> treeEdges;
};