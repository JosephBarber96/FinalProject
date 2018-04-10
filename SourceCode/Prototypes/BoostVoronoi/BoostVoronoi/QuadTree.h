#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class QuadTree
{
public:

	static std::vector<QuadTree*> Children;

	QuadTree();
	QuadTree(int newXOrigin, int newYOrigin, int newWidth, int newHeight, sf::Image populationMap);
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
	virtual bool CheckSplit(sf::Image populationMap);
	virtual void Split(sf::Image populationMap);
};

