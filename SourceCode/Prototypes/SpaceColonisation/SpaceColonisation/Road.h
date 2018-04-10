#pragma once
#include <vector>
class Node;
class Road
{
public:
	Road();
	Road(Node* start);
	~Road();

	void AddPoint(Node* point);

	std::vector<Node*> getPoints() { return points; }
	static std::vector<Road*> getRoads() { return roads; }

private:
	std::vector<Node*> points = std::vector<Node*>();
	static std::vector<Road*> roads;
};

