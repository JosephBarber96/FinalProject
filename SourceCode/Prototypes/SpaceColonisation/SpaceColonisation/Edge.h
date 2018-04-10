#pragma once
#include <vector>
class Node;
class Edge
{
public:
	Edge();
	Edge(Node* par, Node* chi);
	~Edge();

	Node* parent;
	Node* child;
};

