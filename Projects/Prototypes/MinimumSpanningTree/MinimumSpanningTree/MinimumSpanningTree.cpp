#include <iostream>
#include <algorithm>

#include "MinimumSpanningTree.h"
#include "Node.h"
#include "Edge.h"

MinimumSpanningTree::MinimumSpanningTree() {}

MinimumSpanningTree::~MinimumSpanningTree() {}

void MinimumSpanningTree::Sort()
{
	// Completely reset the tree
	treeEdges.clear();
	for (Node* node : nodes)
	{
		node->partOfTree = false;
	}

	// Grab the set of all edges
	std::vector<Edge*> edges = allEdges;

	// Pick a random starting point.
	int nodeIndex = rand() % nodes.size();

	std::vector<Node*> treeNodes;
	treeNodes.push_back(nodes[nodeIndex]);

	bool completed = false;
	while (!completed)
	{
		if (treeNodes.size() > nodes.size()) return;

		// Get the list of all edges connected to any nodes within treeNodes;
		std::vector<Edge*> possibleEdges;
		for (Node* node : treeNodes)
		{
			std::vector<Edge*> edgesForThisNode = GetEdgesForNode(node);

			for (Edge* edge : edgesForThisNode)
			{
				possibleEdges.push_back(edge);
			}
		}

		// Find the lowest length edge that is valid
		Edge* shortestValid = possibleEdges[0];
		int index = 0;
		for (int i = 1; i < possibleEdges.size(); i++)
		{
			// If the end or start of this edge is not contained within the tree
			if (!possibleEdges[0]->start->partOfTree || !possibleEdges[0]->end->partOfTree)
			{
				// And the length is valid
				if (possibleEdges[0]->length < shortestValid->length)
				{
					// Replace...
					shortestValid = possibleEdges[i];
					index = i;
				}
			}
		}
		
		// Add the nodes to the nodes which make up the MST
		if (std::find(treeNodes.begin(), treeNodes.end(), shortestValid->start) != treeNodes.end())
		{
			shortestValid->start->partOfTree = true;
			treeNodes.push_back(shortestValid->start);
		}

		if (std::find(treeNodes.begin(), treeNodes.end(), shortestValid->end) != treeNodes.end())
		{
			shortestValid->end->partOfTree = true;
			treeNodes.push_back(shortestValid->end);
		}

		// All the edge to the MST
		treeEdges.push_back(shortestValid);


		// Check that all nodes are now within TREE
		bool allNodes = true;
		for (Node* node : nodes)
		{
			if (!node->partOfTree)
			{
				allNodes = false;
				break;
			}
		}
		if (allNodes) { completed = true; }
	}

}

void MinimumSpanningTree::Attempt()
{
	// Completely reset the tree
	treeEdges.clear();
	for (Node* node : nodes)
	{
		node->partOfTree = false;
	}

	// Grab the set of all EDGES
	std::vector<Edge*> edges = allEdges;

	for (Edge* edge : allEdges)
	{
		std::cout << "edge length: " << edge->length << std::endl;
	}

	// We also have a subgraph of TREE

	// FOR each edge SHORTEST in EDGES
	bool completed = false;
	while (edges.size() > 2 && !completed)
	{
		// OPTIMIZE
		// SHORTEST = shortest in EDGES
		Edge* shortest = edges[0];
		int index = 0;
		for (int i = 1; i < edges.size(); i++)
		{
			if (edges[i]->length < shortest->length)
			{
				shortest = edges[i];
				index = i;
			}
		}
		edges.erase(edges.begin() + index);

		// If each endpoint of E is not within the subgraph of TREE
		if (!shortest->start->partOfTree || !shortest->end->partOfTree)
		{
			// ADD edge E to TREE
			treeEdges.push_back(shortest);

			shortest->start->partOfTree = true;
			shortest->end->partOfTree = true;
		}

		// Check that all nodes are now within TREE
		bool allNodes = true;
		for (Node* node : nodes)
		{
			if (!node->partOfTree)
			{
				allNodes = false;
				break;
			}
		}
		if (allNodes) { completed = true; }
	}


	std::cout << std::endl;

	//std::cout << "treeEdges size: " << treeEdges.size() << std::endl;

	// Test priority queue

	//bool completed = false;
	//while (!completed)
	//{

	//	Edge* newEdge = sortedEdges.top();
	//	sortedEdges.pop();


	//}

}

void MinimumSpanningTree::AttemptAdjusted()
{
	// Completely reset the tree
	treeEdges.clear();
	for (Node* node : nodes)
	{
		node->partOfTree = false;
	}

	// Grab the set of all EDGES
	std::vector<Edge*> edges = allEdges;

	for (Edge* edge : allEdges)
	{
		std::cout << "edge length: " << edge->length << std::endl;
	}

	// We also have a subgraph of TREE

	// FOR each edge SHORTEST in EDGES
	bool completed = false;
	while (edges.size() > 2 && !completed)
	{
		// OPTIMIZE
		// SHORTEST = shortest in EDGES
		Edge* shortest = edges[0];
		int index = 0;
		for (int i = 1; i < edges.size(); i++)
		{
			if (edges[i]->length < shortest->length)
			{
				shortest = edges[i];
				index = i;
			}
		}
		edges.erase(edges.begin() + index);

		// If each endpoint of E is not within the subgraph of TREE
		if (!shortest->start->partOfTree || !shortest->end->partOfTree)
		{
			// ADD edge E to TREE
			treeEdges.push_back(shortest);

			shortest->start->partOfTree = true;
			shortest->end->partOfTree = true;
		}

		// Check that all nodes are now within TREE
		bool allNodes = true;
		for (Node* node : nodes)
		{
			if (!node->partOfTree)
			{
				allNodes = false;
				break;
			}
		}
		if (allNodes) { completed = true; }
	}


	std::cout << std::endl;

	//std::cout << "treeEdges size: " << treeEdges.size() << std::endl;

	// Test priority queue

	//bool completed = false;
	//while (!completed)
	//{

	//	Edge* newEdge = sortedEdges.top();
	//	sortedEdges.pop();


	//}

}


std::vector<Edge*> MinimumSpanningTree::GetEdgesForNode(Node* node)
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

bool MinimumSpanningTree::TreeEdgesContainsNode(Node* node)
{
	for (Edge* edge : treeEdges)
	{
		if (*edge->start == *node || *edge->end == *node)
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

void MinimumSpanningTree::AddEdge(Edge* newEdge)
{
	allEdges.push_back(newEdge);
}