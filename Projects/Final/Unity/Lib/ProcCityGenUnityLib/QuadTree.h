#pragma once

#include <vector>

class QuadTree
{
public:
	QuadTree();
	QuadTree(int newX, int newY, int newWid, int newHei, QuadTree* _parent);
	virtual ~QuadTree();

	//! Add an object as a child to the head of the tree
	void AddChildToHeadOfTree(QuadTree* child);

	//! Returns the head of the quad tree
	QuadTree* TreeHead();

	//! Getter
	std::vector<QuadTree*> AllChildren() const;

	int XOrigin() const { return xOrigin; }
	int YOrigin() const { return yOrigin; }
	int Width() const { return width; }
	int Height() const { return height; }

protected:
	int xOrigin, yOrigin, width, height;
	QuadTree* parent;
	QuadTree * topLeft, *topRight, *bottomLeft, *bottomRight;
	std::vector<QuadTree*> allChildren;
};