#pragma once
#include <vector>
class V2;
class Node
{
public:
	Node();
	Node(V2* vec);
	Node(int x, int y);
	~Node();

	V2* position;
	std::vector<Node*> getPossibleNeighbours() { return possibleNeighbours; }

	void AddPossibleNeighbour(Node* neighbour);

private:
	std::vector<Node*> possibleNeighbours;


};

