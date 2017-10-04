#pragma once
#include <vector>

class QuadTree
{
public:

	static std::vector<QuadTree*> Children;

	QuadTree();
	QuadTree(int newXOrigin, int newYOrigin, int newWidth, int newHeight);
	~QuadTree();

	int xOrigin;
	int yOrigin;
	int width;
	int height;

	QuadTree* topLeft;
	QuadTree* topRight;
	QuadTree* bottomLeft;
	QuadTree* bottomRight;

private:

	virtual bool CheckSplit();
	virtual void Split();
};

