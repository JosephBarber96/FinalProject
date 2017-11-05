#include "Node.h"
#include "Vec2.h"
#include "Utility.h"
#include "Road.h"

std::vector<Node*> Node::nodes;

Node::Node(int newX, int newY)
{
	pos = new Vec2(newX, newY);

	nodes.push_back(this);
}

Node::Node(Vec2* newPos)
{
	pos = newPos;

	nodes.push_back(this);
}

Node::~Node() {}

void Node::Branch()
{
	Node* shortest = nullptr;

	for (auto node : nodes)
	{
		// Null check
		if (shortest == nullptr && !node->marked)
		{
			//if (Utility::DistanceBetween(node->pos, pos) < maxDist)
			//{
				shortest = node;
			//}	
		}

		if (shortest == nullptr) { continue; }

		// If the distance between myself and node is less than the distance between myself and shortest
		if (Utility::DistanceBetween(node->pos, pos) < Utility::DistanceBetween(shortest->pos, pos)
			// and the node isnt already in the network
			&& !node->marked)
		{
			// Add a max distance
			//if (Utility::DistanceBetween(node->pos, pos) < maxDist)
			//{
				// Check every road
				bool intersect = false;
				for (auto road : Road::getRoads())
				{
					// Check for any intersections
					if (Utility::DoIntersect(road->parent->pos, road->child->pos,
						node->pos, pos))
					{
						intersect = true;
						break;
					}
				}

				// If no intersections
				if (!intersect)
				{
					shortest = node;
				}
			
				
			//}		
		}
	}

	// Null check
	if (shortest != nullptr)
	{ 
		children.push_back(shortest);
		shortest->marked = true;
	}

}