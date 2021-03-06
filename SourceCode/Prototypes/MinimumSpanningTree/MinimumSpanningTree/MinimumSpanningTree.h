#pragma once
#include <vector>

class Node;
class Edge;
class MinimumSpanningTree
{
public:
	MinimumSpanningTree();
	~MinimumSpanningTree();

	std::vector<Edge*> GetEdgesForNode(Node node);

	void SpawnPoints(int numberOfPoints, int minX, int minY, int maxX, int maxY);
	void AssignNighbours(float maxDist);
	void CreateAllEdges();

	void Sort();

	bool TreeEdgesContainsNode(Node node);

	inline std::vector<Node*> GetNodes() { return nodes; }
	inline std::vector<Edge*> GetAllEdges() { return allEdges; }
	inline std::vector<Edge> GetTreeEdges() { return treeEdges; }

private:
	void AddNode(Node* newNode);
	void AddPossibleEdge(Edge* newEdge);

	std::vector<Node*> nodes;
	std::vector<Edge*> allEdges;
	std::vector<Edge> treeEdges;
};