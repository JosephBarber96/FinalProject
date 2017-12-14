#include <vector>
#include <iostream>
#include <math.h>

#include <SFML\Graphics.hpp>

#include "DiamondSquare.h"
#include "QuadTree.h"
#include "MinimumSpanningTree.h"
#include "RoadNode.h"
#include "Node.h"
#include "MstNode.h"
#include "V2.h"
#include "Edge.h"
#include "Road.h"
#include "Pathfinding.h"

const int winSize = 512;
int offsetForRoadNodes = 2;

int main()
{
	/************************
		Population data
	*************************/
	std::string populationMapFileName = "PopulatioNmap.bmp";
	sf::Image populationMap;
	if (!populationMap.loadFromFile(populationMapFileName))
	{
		std::cerr << "Could not find/open populationMap" << std::endl;
		return -1;
	}
	std::cout << "Creating QuadTree..." << std::endl;
	QuadTree* qt = new QuadTree(nullptr, 0, 0, winSize, winSize, populationMap);

	/************************
			MST
	*************************/
	std::cout << "Creating MST..." << std::endl;
	MinimumSpanningTree mst = MinimumSpanningTree();

	for (auto quad : qt->Children())
	{
		mst.SpawnPoint(quad->xOrigin, quad->yOrigin, (quad->xOrigin + quad->width), (quad->yOrigin + quad->height));
	}

	// Assign neighbours
	float maximumDistanceBetweenNeighbours = winSize / 4;
	mst.AssignNighbours(maximumDistanceBetweenNeighbours);

	// Create edges
	mst.CreateAllEdges();

	// Sort the MST
	mst.Sort();

	/************************
		Height/terrain data
	*************************/
	std::cout << "Generating DiamondSquare terrain..." << std::endl;
	int divisions = winSize;
	int size = 5;
	int height = 64;
	DiamondSquare ds = DiamondSquare(divisions, size, height);
	ds.Generate();
	ds.CreatePoints();
	ds.CalcuateBoundaryPoints();
	// ds.Print();

	/****************************************
		Creating roadnodes from terrain data
	*****************************************/
	std::vector<std::vector<RoadNode*>> roadNodes;
	roadNodes = ds.CreatePointsAndPassBackRoadNodes(offsetForRoadNodes);

	/***************************************************
		Pathfind roads using the mst edges
		and roadnodes generated from the diamondsquare
		terrain
	****************************************************/
	std::vector<Road*> roads;
	std::cout << "Pathfinding roads..." << std::endl;
	for (Edge edge : mst.GetTreeEdges())
	{
		if (edge.start->position->x == edge.end->position->x && edge.start->position->y == edge.end->position->y)
		{ 
			continue; 
		}
		Road* road = new Road();
		road->nodes = Pathfinding::PathFind(roadNodes,
			edge.start->position->x / offsetForRoadNodes,
			edge.start->position->y / offsetForRoadNodes,
			edge.end->position->x / offsetForRoadNodes,
			edge.end->position->y / offsetForRoadNodes,
			offsetForRoadNodes);

		roads.push_back(road);
	}

	sf::RenderWindow window(sf::VideoMode(winSize, winSize), "Window");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) { window.close(); }
		}

		window.clear();
		

		/*************************
			Terrain (Heightmap)
		**************************/
		int numOfVerts = ds.Points().size() * ds.Points()[0].size();
		sf::VertexArray vertPoints(sf::Points, numOfVerts);
		int counter = 0;
		for (auto vec : ds.Points())
		{
			for (auto v : vec)
			{
				// Position
				vertPoints[counter].position = sf::Vector2f(v->x, v->y);

				// Color
				sf::Color shapeColor = sf::Color::White;
				int total = ds.Highest();
				float current = v->z;
				current += (abs(ds.Lowest()));
				float percentage = (current * 100) / total;
				float singlePercent = 255 / 100;
				float fillAmount = singlePercent * percentage;
				shapeColor.a = fillAmount;
				vertPoints[counter].color = shapeColor;

				counter++;
			}
		}
		window.draw(vertPoints); 

		/*
			Roads
		*/
		for (Road* road : roads)
		{
			sf::VertexArray roadVertices(sf::LineStrip, road->nodes.size());
			int nodeCounter = 0;
			for (RoadNode* node : road->nodes)
			{
				roadVertices[nodeCounter].position = sf::Vector2f(node->position->x, node->position->y);
				roadVertices[nodeCounter].color = sf::Color(255, 0, 255);
				nodeCounter++;
			}
			window.draw(roadVertices);
		}

		/*
			Points
		*/
		for (auto node : mst.GetNodes())
		{
			sf::CircleShape shape;
			shape.setPosition(node->position->x, node->position->y);
			shape.setFillColor(sf::Color::Green);
			shape.setRadius(1);

			window.draw(shape);
		}

		window.display();
	}

	return 0;
}