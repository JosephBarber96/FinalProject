#pragma once
#include <vector>

class WaterData;
class QuadTree
{
public:
	QuadTree(int _x, int _y, int _wid, int _hei, QuadTree* _par, std::vector<std::vector<float>> popMap, WaterData &waterData, int popMapSize, float highest);
	~QuadTree();

	int xOrigin;
	int yOrigin;
	int width;
	int height;

	// Parent
	QuadTree* parent;

	// Children
	QuadTree* topLeft;
	QuadTree* topRight;
	QuadTree* bottomLeft;
	QuadTree* bottomRight;

	//! Returns the head of the quad tree
	QuadTree* GetHead();
	std::vector<QuadTree*> GetTreeChildren() const { return treeChildren; }

	void AddChildToHeadOfTree(QuadTree* child) { GetHead()->treeChildren.push_back(child); }

private:
	std::vector<QuadTree*> treeChildren;
	bool CheckSplit(std::vector<std::vector<float>> popMap, WaterData &waterData, int size, float highest);
	void Split(std::vector<std::vector<float>> popMap, WaterData &waterData, int size, float highest);
};