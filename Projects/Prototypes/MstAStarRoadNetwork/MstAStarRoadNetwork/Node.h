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

	bool operator== (Node node);

};