#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class QuadTree
{
public:

	std::vector<QuadTree*> Children;

	QuadTree();
	QuadTree(int newXOrigin, int newYOrigin, int newWidth, int newHeight, QuadTree* parent, sf::Image populationMap);
	~QuadTree();

	int xOrigin;
	int yOrigin;
	int width;
	int height;

	QuadTree* topLeft;
	QuadTree* topRight;
	QuadTree* bottomLeft;
	QuadTree* bottomRight;

	QuadTree* parent = nullptr;

	QuadTree* GetHeadOfTree();

private:
	virtual bool CheckSplit(sf::Image populationMap);
	virtual void Split(sf::Image populationMap);
};

