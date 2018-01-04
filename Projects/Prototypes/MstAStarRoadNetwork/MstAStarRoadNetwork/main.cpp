#include <array>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <map>
#include <iostream>

#include <SFML\Graphics.hpp>

#include "AStar.h"
#include "MinimumSpanningTree.h"
#include "Edge.h"
#include "RoadNode.h"
#include "MstNode.h"
#include "Node.h"
#include "Road.h"
#include "V2.h"
#include "QuadTree.h"
#include "debug_mode.h"
#include "DiamondSquare.h"

int winX;
int winY;

MinimumSpanningTree* mst;

bool drawPopulationMap = true;
bool drawQuadTree = true;
bool drawMst = false;
bool drawNodes = true;
bool drawElevationMap = false;
bool drawRoads = true;

void CreateRoadNodesFromSFImage(sf::Image image)
{
	std::cout << "Creating Road Nodes..." << std::endl;

	int imageWidth = image.getSize().x;
	int imageHeight = image.getSize().y;

	int yIndex = 0;
	int xIndex = 0;

	for (int y = 0; y < imageHeight; y++)
	{
		if (y % 5 != 0) { continue; }

		RoadNode::grid.push_back(std::vector<RoadNode*>());
		for (int x = 0; x < imageWidth; x++)
		{

			if (x % 5 != 0) { continue; }

			// Grab the int32 representation of the color
			auto color = image.getPixel(x, y).toInteger();

			// Int32 colours
			// -1061109505 = light grey
			// -2139062017 = dark grey
			// 255 = black

			RoadNode* node = new RoadNode(x, y);
			node->yIndex = yIndex;
			node->xIndex = xIndex;

			switch (color)
			{
			case -1061109505:
				node->SetCost(20);
				break;
			case -2139062017:
				node->SetCost(50);
				break;
			case 255:
				node->SetCost(100);
				break;
			default:
				node->SetCost(0);
				break;
			}

			RoadNode::grid[yIndex].push_back(node);

			xIndex++;
		}
		yIndex++;
		xIndex = 0;
	}

	for (auto vec : RoadNode::grid)
	{
		for (auto node : vec)
		{
			node->FillNeighbours();
		}
	}

	//for (int y = 0; y < imageHeight - 1; y++)
	//{
	//	for (int x = 0; x < imageHeight - 1; x++)
	//	{
	//		RoadNode::grid[y][x]->FillNeighbours(ySize - 1, xSize - 1);
	//	}
	//}

	std::cout << "RoadNodes setup successfully" << std::endl;
}

void CreateRoadNodesFromDiaSqu(std::vector<std::vector<RoadNode>> roadNodes, DiamondSquare ds)
{
	std::cout << "Creating Road Nodes..." << std::endl;

	int imageWidth = 1024;
	int imageHeight = 1024;

	int yIndex = 0;
	int xIndex = 0;

	for (int y = 0; y < imageHeight; y++)
	{
		if (y % 5 != 0) { continue; }

		/*if (y % 100 == 0)*/ { std::cout << "Creating on y = " << y << std::endl; }

		// RoadNode::grid.push_back(std::vector<RoadNode*>());
		// roadNodes.push_back(std::vector<RoadNode>());
		for (int x = 0; x < imageWidth; x++)
		{
			if (x % 5 != 0) { continue; }

			// Grab the cost of this node
			// auto cost = (ds.Points()[y][x]->z + abs(ds.Lowest())) * 2;

			//RoadNode objnode = RoadNode(x, y);
			//objnode.xIndex = xIndex;
			//objnode.yIndex = yIndex;
			//objnode.SetCost(cost);
			// xIndex++;

			//RoadNode* node = new RoadNode(x, y);
			//node->yIndex = yIndex;
			//node->xIndex = xIndex;

			//node->SetCost(cost);

			//RoadNode::grid[yIndex].push_back(node);

			xIndex++;
		}
		yIndex++;
		xIndex = 0;
	}

	std::cout << "Filling neighbours..." << std::endl;

	for (auto vec : RoadNode::grid)
	{
		for (auto node : vec)
		{
			node->FillNeighbours();
		}
	}

	std::cout << "RoadNodes setup successfully" << std::endl;
}

void SearchImageForPixelData(sf::Image image)
{
	int xSize = image.getSize().x;
	int ySize = image.getSize().y;

	int counter = 0;
	std::map<int, int> colorMap;

	for (int y = 0; y < ySize; y++)
	{
		for (int x = 0; x < xSize; x++)
		{
			// Grab the int32 representation of the color
			auto color = image.getPixel(x, y).toInteger();

			if (colorMap.find(color) == colorMap.end())
			{
				colorMap.insert(std::pair<int, int>(color, counter));
				counter++;
			}
			else
			{
				colorMap[color]++;
			}
		}
	}

	std::cout << "Number of different pixel colours:" << colorMap.size() << std::endl;
	std::map<int, int>::iterator iter;
	for (iter = colorMap.begin(); iter != colorMap.end(); iter++)
	{
		std::cout << "Colour key: " << iter->first << " entries: " << iter->second << std::endl;
	}
}

int main()
{
	// Seed random
	srand(time(NULL));

	// Read in population data
	std::string populationMapFileName = "PopulationMap.bmp";
	sf::Image populationMap;
	if (!populationMap.loadFromFile(populationMapFileName))
	{
		std::cerr << "Could not find/open populationMap" << std::endl;
		return -1;
	}

	// Create a window
	winX = populationMap.getSize().x;
	winY = populationMap.getSize().y;
	std::cout << "Win_x: " << winX << " Win_y: " << winY << std::endl;

	// Create a sprite to display this image;
	sf::Texture populationTexture;
	if (!populationTexture.loadFromFile(populationMapFileName))
	{
		return -1;
	}
	populationTexture.setSmooth(true);

	sf::Sprite populationSprite;
	populationSprite.setTexture(populationTexture);
	populationSprite.setPosition(0, 0);


	// Create a quad tree
	QuadTree* qt = new QuadTree(0, 0, winX, winY, populationMap);

	// Minimum Spanning tree
	mst = new MinimumSpanningTree();

	// Plot points
	for (QuadTree* qt : QuadTree::Children)
	{
		mst->SpawnPoint(qt->xOrigin, qt->yOrigin, (qt->xOrigin + qt->width), (qt->yOrigin + qt->height));
	}

#if DEBUG_MODE == false
	for (auto node : mst->GetNodes())
	{
		if (node->position->x == winX)
		{
			node->position->x -= 5;
		}
		
		if (node->position->y == winY)
		{
			node->position->y -= 5;
		}

		//if (node->position->x == winX || node->position->y == winY)
		//{
		//	std::cerr << "Node position is at the end of the map" << std::endl;
		//	return -1;
		//}

		if (node->position->x % 5 != 0 || node->position->y % 5 != 0)
		{
			std::cerr << "Node position not set on a factor of 5" << std::endl;
			return -1;
		}
	}
#endif

	// Assign neighbour
	float maxDist = winX / 4;
	mst->AssignNighbours(maxDist);

	// Edges
	mst->CreateAllEdges();

	// Sort the MST
	mst->Sort();
	std::cout << "The quad tree consts of " << QuadTree::Children.size() << " quads." << std::endl;
	std::cout << "The complete MST has " << mst->GetTreeEdges().size() << " edges." << std::endl;

	// Now that we've sorted the MST, we have to set up road nodes and use A* to pathfind the roads.
	// Create nodes for the roads...

	/********************************************
		Create roads from a heightmap using SFML
	*********************************************/

	//std::string elevationMapFileName = "elevationMap.bmp";
	//sf::Image evelationMap;
	//if (!evelationMap.loadFromFile(elevationMapFileName))
	//{
	//	std::cerr << "Could not find/open elevationMap" << std::endl;
	//	return -1;
	//}

	//// Create a sprite to display this image;
	//sf::Texture elevationTexture;
	//if (!elevationTexture.loadFromFile(elevationMapFileName))
	//{
	//	return -1;
	//}
	//elevationTexture.setSmooth(true);

	//sf::Sprite elevationSprite;
	//elevationSprite.setTexture(elevationTexture);
	//elevationSprite.setPosition(0, 0);

	//CreateRoadNodesFromSFImage(evelationMap);


	/**************************************************************
		Create roads from a diamondsquare terrain (added 12/12/17)
	***************************************************************/

	int divisions = 1024;
	int size = 5;
	int height = 50;
	std::cout << "Generating DiamondSquare terrain." << std::endl;
	DiamondSquare ds = DiamondSquare(divisions, size, height);
	ds.Generate();
	ds.CreatePoints();
	ds.CalcuateBoundaryPoints();

	std::vector<std::vector<RoadNode>> roadNodes;
	CreateRoadNodesFromDiaSqu(roadNodes, ds);

	// Construct a road from each edge
	int counter = 0;
	for (Edge edge : mst->GetTreeEdges())
	{
		std::cout << "Creating node for edge: " << ++counter << std::endl;
		// Make a road for this edge
		Road* road = new Road();

		// Pathfind it
		road->nodes = AStar::PathFind(edge.start->position->x, edge.start->position->y, edge.end->position->x, edge.end->position->y);
	}

	sf::RenderWindow window(sf::VideoMode(winX, winY), "Road network prototype");

	// Display
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) { window.close(); }
			else if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Num1:
					drawPopulationMap = !drawPopulationMap;
					break;
				case sf::Keyboard::Num2:
					drawQuadTree = !drawQuadTree;
					break;
				case sf::Keyboard::Num3:
					drawMst = !drawMst;
					break;
				case sf::Keyboard::Num4:
					drawNodes = !drawNodes;
					break;
				case sf::Keyboard::Num5:
					drawElevationMap = !drawElevationMap;
					break;
				case sf::Keyboard::Num6:
					drawRoads = !drawRoads;
					break;
				}
			}
		}

		// Clear
		window.clear();

		// Draw...

		// Population map
		if (drawPopulationMap)
		{
			window.draw(populationSprite);
		}

		// Elevation map
		if (drawElevationMap)
		{
			// window.draw(elevationSprite);
		}

		// Quad tree
		if (drawQuadTree)
		{
			for (QuadTree* qt : QuadTree::Children)
			{
				sf::Vertex qtVertices[5] =
				{
					sf::Vertex(sf::Vector2f(qt->xOrigin, qt->yOrigin), sf::Color::Red),
					sf::Vertex(sf::Vector2f(qt->xOrigin, qt->yOrigin + qt->height), sf::Color::Red),
					sf::Vertex(sf::Vector2f(qt->xOrigin + qt->width, qt->yOrigin + qt->height), sf::Color::Red),
					sf::Vertex(sf::Vector2f(qt->xOrigin + qt->width, qt->yOrigin), sf::Color::Red),
					sf::Vertex(sf::Vector2f(qt->xOrigin, qt->yOrigin), sf::Color::Red)
				};

				window.draw(qtVertices, 5, sf::LineStrip);
			}
		}


		// MST
		if (drawMst)
		{
			for (Edge edge : mst->GetTreeEdges())
			{
				sf::Vertex edgeVertices[2] =
				{
					sf::Vertex(sf::Vector2f(edge.start->position->x, edge.start->position->y), sf::Color::Blue),
					sf::Vertex(sf::Vector2f(edge.end->position->x, edge.end->position->y), sf::Color::Blue)
				};

				window.draw(edgeVertices, 2, sf::Lines);
			}
		}

		if (drawRoads)
		{
			for (Road* road : Road::roads)
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
		}

		if (drawNodes)
		{
			// Nodes
			for (MstNode* node : mst->GetNodes())
			{
				sf::CircleShape shape;
				shape.setRadius(1);
				shape.setPosition(node->position->x, node->position->y);
				shape.setFillColor(sf::Color::Green);

				window.draw(shape);
			}
		}

		// Display
		window.display();
	}


	return 0;
}