#include "Defines.h"

#if DEBUG_MODE == 1
#include <iostream>
#endif

#include <algorithm>
#include <queue>
#include <map>
#include <stdlib.h>

#include "MinimumSpanningTree.h"
#include "Node.h"
#include "Edge.h"
#include "V2.h"

MinimumSpanningTree::MinimumSpanningTree() {}

MinimumSpanningTree::~MinimumSpanningTree() {}

void MinimumSpanningTree::SpawnPoints(int numberOfPoints, int minX, int minY, int maxX, int maxY)
{

	for (int i = 0; i < numberOfPoints; i++)
	{
		int nodeX = rand() % maxX - minX;
		int nodeY = rand() % maxY - minY;
		Node* node = new Node(nodeX, nodeY);
		node->id = i;
		AddNode(node);

#if DEBUG_MODE == true
		std::cout << "Node created at: " << node->position->x << ", " << node->position->y << std::endl;
#endif
	}
}

void MinimumSpanningTree::AssignNighbours(float maxDist)
{
	// For each node
	for (Node* node : nodes)
	{

		bool neighbourFound = false;
		Node* closestNeighbour = nullptr;

		// For every other ndoe
		for (Node* possibleNeighbour : nodes)
		{
			// Skip itself
			if (*node == *possibleNeighbour) { continue; }

			// The first neighbour is the closest if closestNeighbour is nullptr
			if (closestNeighbour == nullptr)
			{
				closestNeighbour = possibleNeighbour;
				continue;
			}

			// If possible neighbour is closer than closest neighbour, replace it
			if (V2::DistanceBetween(*node->position, *possibleNeighbour->position) < V2::DistanceBetween(*node->position, *closestNeighbour->position))
			{
				closestNeighbour = possibleNeighbour;
			}

			// If possibleNeighbour is close enough, add it as a possible neighbour
			if (V2::DistanceBetween(*node->position, *possibleNeighbour->position) < maxDist)
			{
				neighbourFound = true;
				node->AddPossibleNeighbour(possibleNeighbour);
			}
		}

		// After checking each neighbour, if a close enough neighbour wasnt found, add the closest neighbour
		if (!neighbourFound)
		{
			node->AddPossibleNeighbour(closestNeighbour);
		}
	}
}

void MinimumSpanningTree::CreateAllEdges()
{
	// For every node
	for (Node* node : nodes)
	{
		// For each neighbour
		for (Node* neighbour : node->getPossibleNeighbours())
		{
			// Create an edge between the node and neighbour
			Edge* edge = new Edge(node, neighbour);

			if (GetAllEdges().empty())
			{
				AddPossibleEdge(edge);
				continue;
			}
			else
			{

				if (std::find(allEdges.begin(), allEdges.end(), edge) != allEdges.end())
				{
					continue;
				}
				else
				{
					AddPossibleEdge(edge);
				}
			}
		}
	}

#if DEBUG_MODE == true
	std::cout << "Total number of edges: " << allEdges.size() << std::endl;
	for (Edge* edge : allEdges)
	{
		std::cout << "Edge - Start: (" << edge->start->position->x << ", " << edge->start->position->y << "), End(" << edge->end->position->x << ", " << edge->end->position->y << ")" << std::endl;
	}
#endif

}

void MinimumSpanningTree::Sort()
{
	// Completely reset the tree
	treeEdges.clear();
	for (Node* node : nodes)
	{
		node->partOfTree = false;
	}
	for (Edge* edge : allEdges)
	{
		edge->partOfTree = false;
	}

	// Start

	// Find the edge of lowest cost
	Edge shortest = *allEdges[0];
	for (int i = 1; i < allEdges.size(); i++)
	{
		if (allEdges[i]->length < shortest.length)
		{
			shortest = *allEdges[i];
		}
	}

	treeEdges.push_back(shortest);
	shortest.partOfTree = true;
	shortest.start->partOfTree = true;
	shortest.end->partOfTree = true;

	// Dictionary for debugging to keep count of how many times each node was added.
	std::map<int, int> myMap;
	for (Node* node : nodes)
	{
		myMap.insert(std::pair<int, int>(node->id, 0));
	}

	// Loop...
	bool completed = false;
	int loopCount = 0;
	while (!completed)
	{
		loopCount++;

		// Get every node connected to the MST
		std::vector<Node> mstNodes;
		for (Edge edge : treeEdges)
		{
			// If edge.start isnt in mstNodes... add it
			if (std::find(mstNodes.begin(), mstNodes.end(), *edge.start) != mstNodes.end())
			{

			}
			else
			{
				// std::cout << "Adding node " << edge.start->id << " to grid on loop " << loopCount << std::endl;
				myMap[edge.start->id] += 1;
				mstNodes.push_back(*edge.start);
			}

			// If edge.end isnt in mstNodes... add it
			if (std::find(mstNodes.begin(), mstNodes.end(), *edge.end) != mstNodes.end())
			{

			}
			else
			{
				// std::cout << "Adding node " << edge.end->id << " to grid on loop " << loopCount << std::endl;
				myMap[edge.end->id] += 1;
				mstNodes.push_back(*edge.end);
			}
		}


		// Get all edges for these nodes that arent already within the MST and are valid
		std::vector<Edge> possibleEdges;
		// For each node
		for (Node node : mstNodes)
		{
			// Get all edges connected to this road
			std::vector<Edge*> nodeEdges = GetEdgesForNode(node);

			// For each edge connected to this node
			for (Edge* edge : nodeEdges)
			{
				// If the edge is already a part of the tree, exit
				if (edge->partOfTree) continue;

				// If one side of the edge is not in the tree (thus can be connected to from the current tree)
				// add it to possibleEdges

				// If start and not end
				if (edge->start->partOfTree && !edge->end->partOfTree)
				{
					possibleEdges.push_back(*edge);
				}
				// Or end and not start
				else if (edge->end->partOfTree && !edge->start->partOfTree)
				{
					possibleEdges.push_back(*edge);
				}
			}
		}

		// Grab the shortest possibleEdge
		Edge theShortest = possibleEdges[0];
		if (possibleEdges.size() > 1)
		{
			for (int i = 0; i < possibleEdges.size(); i++)
			{
				if (possibleEdges[i].length < theShortest.length)
				{
					theShortest = possibleEdges[i];
				}
			}
		}

		// Add the shortest to the MST...
		treeEdges.push_back(theShortest);

		// Add the nodes to MST nodes...
		if (!(std::find(mstNodes.begin(), mstNodes.end(), *theShortest.start) != mstNodes.end()))
		{
			mstNodes.push_back(*theShortest.start);
		}

		if (!(std::find(mstNodes.begin(), mstNodes.end(), *theShortest.end) != mstNodes.end()))
		{
			mstNodes.push_back(*theShortest.end);
		}

		//Mark them
		theShortest.partOfTree = true;
		theShortest.start->partOfTree = true;
		theShortest.end->partOfTree = true;


		// Check to see if all nodes are now a part of the tree..
		bool allInTree = true;
		for (Node* node : nodes)
		{
			if (!node->partOfTree)
			{
				allInTree = false;
				break;
			}
		}

		// If they all are in MST...
		if (allInTree)
		{
			// stop looping
			completed = true;
		}
	}

	// Debugging - output how many times each was added
	for (Node* node : nodes)
	{
#if DEBUG_MODE == true
		std::cout << "Node " << node->id << " has " << myMap[node->id] << " entries." << std::endl;
#endif
	}
}

std::vector<Edge*> MinimumSpanningTree::GetEdgesForNode(Node node)
{
	std::vector<Edge*> edges;

	for (Edge* edge : allEdges)
	{
		if (edge->EdgeContainsNode(node))
		{
			edges.push_back(edge);
		}
	}

	return edges;
}

bool MinimumSpanningTree::TreeEdgesContainsNode(Node node)
{
	for (Edge edge : treeEdges)
	{
		if (*edge.start == node || *edge.end == node)
		{
			return true;
		}
	}
	return false;
}

void MinimumSpanningTree::AddNode(Node* newNode)
{
	nodes.push_back(newNode);
}

void MinimumSpanningTree::AddPossibleEdge(Edge* newEdge)
{
	allEdges.push_back(newEdge);
}