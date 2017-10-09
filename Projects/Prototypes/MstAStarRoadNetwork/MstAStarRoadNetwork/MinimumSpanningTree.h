#pragma once
#include <vector>
#include <set>

class Node;
class Edge;
class MinimumSpanningTree
{
public:
	MinimumSpanningTree();
	~MinimumSpanningTree();

	std::vector<Edge*> GetEdgesForNode(Node node);

	void SpawnPoints(int numberOfPoints, int minX, int minY, int maxX, int maxY);
	void SpawnPoint(int minX, int minY, int maxX, int maxY);
	void AssignNighbours(float maxDist);
	void CreateAllEdges();

	void Sort();

	bool TreeEdgesContainsNode(Node node);

	inline std::vector<Node*> GetNodes() { return nodes; }
	inline std::set<Edge*> GetAllEdges() { return allEdges; }
	inline std::vector<Edge> GetTreeEdges() { return treeEdges; }

private:
	void AddNode(Node* newNode);
	void AddPossibleEdge(Edge* newEdge);

	int nodeCount = 0;

	std::vector<Node*> nodes;
	std::set<Edge*> allEdges;
	std::vector<Edge> treeEdges;
};