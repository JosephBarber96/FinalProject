#include <iostream>
#include "Node.h"
#include "Vec2.h"
#include "Utility.h"
#include "Edge.h"

Node::Node(int newX, int newY)
{
	pos = new Vec2(newX, newY);
}

Node::Node(Vec2* newPos)
{
	pos = newPos;
}

Node::~Node() {}

void Node::Branch(std::vector<Node*> nodes, std::vector<Edge*> edges)
{
	// Shortest = null
	Node* shortest = nullptr;

	float minDist = 75;

	// For every node...
	for (auto neighbour : nodes)
	{
		if (neighbour->start) { continue; }

		// If shortest is null
		if (shortest == nullptr)
		{
			// If neighbour doesn't have a parent
			if (neighbour->parent == nullptr)
			{
				// And is within the minDistance...
				if (Utility::DistanceBetween(neighbour->getPos(), pos) < minDist)
				{
					// And it doesn't create a road intersection...
					bool intersection = false;
					// For each road
					for (auto edge : edges)
					{
						if (Utility::Intersect(neighbour->getPos(), pos, edge->child->getPos(), edge->parent->getPos()))
						{
							// std::cout << "Intersection found whilst assigning shortest. Breaking." << std::endl;
							intersection = true;
							break;
						}
					}

					// If no intersection...
					if (!intersection)
					{
						// It becomes the shortest
						shortest = neighbour;
					}
				}
			}
		}

		// Else
		else
		{
			// If neighbour doesn't already have a parent
			if (neighbour->parent == nullptr)
			{
				// If neighbour is closer than shortset
				if (Utility::DistanceBetween(neighbour->getPos(), pos) < Utility::DistanceBetween(shortest->getPos(), pos))
				{

					// If this connection doesn't create an road intersection...
					bool intersection = false;
					// For each road
					for (auto edge : edges)
					{
						if (Utility::Intersect(neighbour->getPos(), pos, edge->child->getPos(), edge->parent->getPos()))
						{
							// std::cout << "Intersection found. Breaking." << std::endl;
							intersection = true;
							break;
						}
					}

					// If no intersection...
					if (!intersection)
					{
						// neighbour is our new shortest
						shortest = neighbour;
					}
				}
			}
		}
	}

	// If shortest still isn't null
	if (shortest != nullptr)
	{
		// Add shortest to our children
		children.push_back(shortest);

		// Shortest's parent is this
		shortest->parent = this;
	}
}