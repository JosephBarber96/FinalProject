#pragma once
#include <vector>
class Vec2;
class Edge;
class Node
{
public:
	Node(int newX, int newY);
	Node(Vec2* newPos);
	~Node();

	std::vector<Node*> children;
	Node* parent = nullptr;
	bool start = false;

	Vec2* getPos() { return pos; }

	void Branch(std::vector<Node*> nodes, std::vector<Edge*> edges);

private:
	Vec2* pos;
};

