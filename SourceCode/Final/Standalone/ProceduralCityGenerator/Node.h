#pragma once
class Vec2;
class Node
{
public:
	Node();
	Node(int ix, int iy);
	Node(float fx, float fy);
	Node(Vec2* pos);
	~Node();

	Vec2* position;

	bool operator== (Node rhs);
};