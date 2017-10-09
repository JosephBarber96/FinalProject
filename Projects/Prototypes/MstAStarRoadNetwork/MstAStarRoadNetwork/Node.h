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

	int id;
	V2* position;
	bool partOfTree = false;
	bool checkedForNeighbours = false;

	std::vector<Node*> getPossibleNeighbours() { return possibleNeighbours; }

	void AddPossibleNeighbour(Node* neighbour);

	bool operator== (Node node);

private:
	std::vector<Node*> possibleNeighbours;
};

