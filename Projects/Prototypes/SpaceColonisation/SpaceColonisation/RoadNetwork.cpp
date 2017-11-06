#include "RoadNetwork.h"
#include "Node.h"
#include "Road.h"
#include "Edge.h"
#include "Vec2.h"
#include "Utility.h"


RoadNetwork::RoadNetwork(Road* newMainRoad)
	:
	mainRoad(newMainRoad)
{}

RoadNetwork::~RoadNetwork() {}

void RoadNetwork::AddNode(Node* newNode)
{
	nodes.push_back(newNode);
}

void RoadNetwork::AddEdge(Edge* newEdge)
{
	edges.push_back(newEdge);
}

void RoadNetwork::Generate()
{
	std::vector<Node*> mainRoadNodes;

	int counter = -1;
	for (auto node : mainRoad->getPoints())
	{
		counter++;
		if (counter == 0 || counter == mainRoad->getPoints().size()) { continue; }

		mainRoadNodes.push_back(node);
	}

	Generate(mainRoadNodes);
}

void RoadNetwork::Generate(Node* start)
{
	std::list<Node*> list = std::list<Node*>();
	list.push_back(start);
	Generate(list);
}

void RoadNetwork::Generate(std::vector<Node*> startingNodes)
{
	std::list<Node*> startingList = std::list<Node*>();
	for (auto node : startingNodes)
	{
		startingList.push_back(node);
	}
	Generate(startingList);
}

void RoadNetwork::Generate(std::list<Node*> startingNodes)
{
	// Create a list to hold the nodesToProcess on the current iteration
	std::list<Node*> nodesToProcess = startingNodes;

	// Set these nodes as starting nodes
	for (auto node : nodesToProcess)
	{
		node->start = true;
	}

	// Create a list to hold nodesToCheck on the next iteration
	std::list<Node*> nodesToCheckNext = std::list<Node*>();

	bool quit = false;
	while (!quit)
	{
		// For every node to process in this loop
		for (auto node : nodesToProcess)
		{
			// Branch the node twice to find 2 children
			for (int i = 0; i < 2; i++) { node->Branch(nodes, edges); }

			// If it found children...
			if (!node->children.empty())
			{
				// For each new child
				for (auto child : node->children)
				{
					// Add it to nodesToCheckNext
					nodesToCheckNext.push_back(child);

					// Create a road to represent this connection
					Edge* edge = new Edge(node, child);
					AddEdge(edge);
				}
			}
		}

		// nodesToProcess is now replaced with nodesToCheckNext
		nodesToProcess = nodesToCheckNext;

		// nodesToCheckNext is emptied
		while (!nodesToCheckNext.empty()) { nodesToCheckNext.pop_front(); }
		nodesToCheckNext = std::list<Node*>();

		// If we have no more nodes to process, we quit
		if (nodesToProcess.empty()) { quit = true; }
	}
}

void RoadNetwork::ChildOrphanNodes()
{
	std::list<Node*> orphanNodes = std::list<Node*>();
	
	// For every node
	for (auto node : nodes)
	{
		// If it's parent is null, and it's not a starting node
		if (node->parent == nullptr && !node->start)
		{
			// Add it into our list of orphans
			orphanNodes.push_back(node);
		}
	}

	Node* inTree = nullptr;
	Node* orphan = nullptr;
	float minDist = INT_MAX;

	// For each node with a parent
	for (auto treeNode : nodes)
	{
		if (treeNode->parent == nullptr && !treeNode->start) { continue; }

		// For each orphan node
		for (auto orphanNode : orphanNodes)
		{
			// Find the closest orphan-tree connection
			float dist = Utility::DistanceBetween(treeNode->getPos(), orphanNode->getPos());
			if (dist < minDist)
			{
				inTree = treeNode;
				orphan = orphanNode;
				minDist = dist;
			}
		}
	}

	// Null checks
	if (inTree == nullptr || orphan == nullptr) { return; }

	// Once we have this, create the road
	Edge* edge = new Edge(inTree, orphan);
	AddEdge(edge);

	// Generate again, starting from the orphan
	Generate(orphan);
}