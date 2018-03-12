#include "QuadTree.h"

QuadTree::QuadTree() {}

QuadTree::QuadTree(int newX, int newY, int newWid, int newHei, QuadTree* newParent)
	:
	xOrigin(newX),
	yOrigin(newY),
	width(newWid),
	height(newHei),
	parent(newParent)
{}

QuadTree::~QuadTree() {}

void QuadTree::AddChildToHeadOfTree(QuadTree* child)
{
	TreeHead()->allChildren.push_back(child);
}

QuadTree* QuadTree::TreeHead()
{
	if (parent != nullptr)
	{
		return parent->TreeHead();
	}
	return this;
}

std::vector<QuadTree*> QuadTree::AllChildren() const
{
	return allChildren;
}