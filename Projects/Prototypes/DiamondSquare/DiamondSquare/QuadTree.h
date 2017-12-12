#pragma once
#include <vector>

#include <SFML\Graphics.hpp>

class QuadTree
{
public:
	QuadTree(QuadTree* parent, int newXOrigin, int newYOrigin, int newWidth, int newHeight, sf::Image populationMap);
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

	QuadTree* GetHead();
	void AddChild(QuadTree* child) { children.push_back(child); }

	std::vector<QuadTree*> Children() { return children; }

private:
	std::vector<QuadTree*> children;

	virtual bool CheckSplit(sf::Image populationMap);
	virtual void Split(sf::Image populationMap);
};

