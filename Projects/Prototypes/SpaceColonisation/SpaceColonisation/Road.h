#pragma once
#include <vector>
class Node;
class Road
{
public:
	Road();
	Road(Node* par, Node* chi);
	~Road();

	Node* parent;
	Node* child;

	static std::vector<Road*> getRoads() { return roads; }

private:
	static std::vector<Road*> roads;
};

