#pragma once
#include <vector>
class Node
{
public:
	Node(int newX, int newY);
	~Node();

	static std::vector<Node*> nodes;


	int x;
	int y;
};

