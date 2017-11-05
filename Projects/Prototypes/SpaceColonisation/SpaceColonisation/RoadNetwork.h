#pragma once
class Node;
class RoadNetwork
{
public:
	RoadNetwork();
	~RoadNetwork();

	void Generate(Node* start);
};

