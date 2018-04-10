#pragma once
#include <list>
#include <vector>
class Node;
class Road;
class Edge;
class RoadNetwork
{
public:
	RoadNetwork(Road* newMainRoad);
	~RoadNetwork();

	void AddNode(Node* newNode);
	void AddEdge(Edge* newEdge);
	void Generate();
	void Generate(Node* start);
	void Generate(std::vector<Node*> startingNodes);
	void Generate(std::list<Node*> startingNodes);
	void ChildOrphanNodes();

	Road* getMainRoad() { return mainRoad; }
	std::vector<Edge*> getEdges() { return edges; }
	std::vector<Node*> getNodes() { return nodes; }

private:
	Road* mainRoad;
	std::vector<Edge*> edges;
	std::vector<Node*> nodes;
};

