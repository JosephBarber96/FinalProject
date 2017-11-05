#pragma once
#include <vector>
class Vec2;
class Node
{
public:
	Node(int newX, int newY);
	Node(Vec2* newPos);
	~Node();

	std::vector<Node*> children;
	bool marked = false;

	Vec2* getPos() { return pos; }
	static std::vector<Node*> getNodes() { return nodes; }

	void Branch();

private:
	//! List of all nodes
	static std::vector<Node*> nodes;
	Vec2* pos;

};

