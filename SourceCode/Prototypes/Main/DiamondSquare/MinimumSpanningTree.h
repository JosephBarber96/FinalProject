#pragma once
#include <vector>
#include <set>

class MstNode;
class Edge;
class WaterData;
class MinimumSpanningTree
{
public:
	MinimumSpanningTree();
	~MinimumSpanningTree();

	std::vector<Edge*> GetEdgesForNode(MstNode node);

	void SpawnPoints(int numberOfPoints, int minX, int minY, int maxX, int maxY);
	void SpawnPoint(WaterData &wd, int offset, int minX, int minY, int maxX, int maxY);
	void AssignNighbours(float maxDist);
	void CreateAllEdges();

	void Sort();

	bool TreeEdgesContainsNode(MstNode node);

	inline std::vector<MstNode*> GetNodes() { return nodes; }
	inline std::set<Edge*> GetAllEdges() { return allEdges; }
	inline std::vector<Edge> GetTreeEdges() { return treeEdges; }

private:
	void AddNode(MstNode* newNode);
	void AddPossibleEdge(Edge* newEdge);

	int nodeCount = 0;

	std::vector<MstNode*> nodes;
	std::set<Edge*> allEdges;
	std::vector<Edge> treeEdges;
};