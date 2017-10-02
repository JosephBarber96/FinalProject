#include "MinimumSpanningTree.h"

MinimumSpanningTree::MinimumSpanningTree() {}

MinimumSpanningTree::~MinimumSpanningTree() {}

void MinimumSpanningTree::AddNode(Node* newNode)
{
	nodes.push_back(newNode);
}

void MinimumSpanningTree::AddEdge(Edge* newEdge)
{
	allEdges.push_back(newEdge);
}