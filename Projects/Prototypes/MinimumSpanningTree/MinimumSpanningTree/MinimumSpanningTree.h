#pragma once
#include <vector>

class Node;
class Edge;
class MinimumSpanningTree
{
public:
	MinimumSpanningTree();
	~MinimumSpanningTree();

	void AddNode(Node* newNode);
	void AddEdge(Edge* newEdge);

	void Sort();
	void Attempt();
	void AttemptAdjusted();

	std::vector<Edge*> GetEdgesForNode(Node* node);

	bool TreeEdgesContainsNode(Node* node);

	inline std::vector<Node*> GetNodes() { return nodes; }
	inline std::vector<Edge*> GetAllEdges() { return allEdges; }
	inline std::vector<Edge*> GetTreeEdges() { return treeEdges; }

private:

	std::vector<Node*> nodes;
	std::vector<Edge*> allEdges;
	std::vector<Edge*> treeEdges;
};