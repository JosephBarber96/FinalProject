#include <list>

#include "RoadNetwork.h"
#include "Node.h"
#include "Road.h"
#include "Vec2.h"
#include "Utility.h"


RoadNetwork::RoadNetwork() {}

RoadNetwork::~RoadNetwork() {}

void RoadNetwork::Generate(Node* start)
{
	std::list<Node*> nodesToProcess = std::list<Node*>();

	nodesToProcess.push_back(start);
	start->marked = true;

	bool quit = false;

	while (!quit)
	{
		// Get the front of nodesToProcess
		Node* node = nodesToProcess.front();
		// Remove it from the list
		nodesToProcess.pop_front();

		// This node will try to find 2 children
		for (int i = 0; i < 2; i++)
		{
			node->Branch();
		}

		// If it found children
		if (node->children.size() > 0)
		{
			// Add these children to the back of list
			for (auto child : node->children)
			{
				nodesToProcess.push_back(child);

				// Create a road from this connection
				Road* road = new Road(node, child);
			}
		}
		

		// Check if we're done
		if (nodesToProcess.empty()) { quit = true; }
	}
}