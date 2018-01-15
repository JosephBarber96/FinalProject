#include <vector>
#include <iostream>
#include <math.h>

#include "FastNoise.h"
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
#include "WaterData.h"
#include "BuildingLot.h"

const int winSize = 512;
int offsetForRoadNodes = 5;

void makeAllFastNoiseValuesPositive(std::vector<std::vector<float>> &popMap, float &highestVal)
{
	// Find the lowest and highest values
	float highest = INT_MIN;
	float lowest = INT_MAX;
	for (auto vec : popMap)
	{
		for (auto val : vec)
		{
			if (val > highest) highest = val;
			if (val < lowest) lowest = val;
		}
	}

	highestVal = highest;

	// Add the absolute of the lowest onto every value
	// so it ranges from 0-highest+abs(lowest) and not -1 - 1
	float toAdd = fabsf(lowest);
	highestVal += toAdd;
	for (auto &vec : popMap)
	{
		for (auto &val : vec)
		{
			val += toAdd;
		}
	}

}

void seedNoise(FastNoise &fn)
{
	srand(time(NULL));

	fn.SetSeed(rand() % INT_MAX);
}

void fillNoise(FastNoise &fn, std::vector<std::vector<float>> &popMap, int size)
{
	for (int y = 0; y < size; y++)
	{
		popMap.push_back(std::vector<float>());
		for (int x = 0; x < size; x++)
		{
			popMap[y].push_back(fn.GetNoise(x, y));
		}
	}
}

void LoadWaterData(WaterData &wd)
{
	sf::Image waterMap;
	waterMap.loadFromFile("watermap.bmp");

	int wid = waterMap.getSize().x;
	int hei = waterMap.getSize().y;

	for (int y = 0; y < hei; y++)
	{
		for (int x = 0; x < wid; x++)
		{
			waterMap.getPixel(x, y) == sf::Color::White ? wd.SetPixelWater(x, y, true) : wd.SetPixelWater(x, y, false);
		}
	}
}

int main()
{

	/*************************
		Population noise
	**************************/
	std::cout << "Generation population noise." << std::endl;
	FastNoise fn;
	std::vector<std::vector<float>> popMap;
	seedNoise(fn);
	fn.SetNoiseType(FastNoise::ValueFractal);
	fn.SetFrequency(0.01); // Default 0.01
	fn.SetFractalOctaves(6); // Default 3
	fn.SetInterp(FastNoise::Interp::Quintic); // Default Quintic
	fillNoise(fn, popMap, winSize);

	/* Make the values positive */
	float highestVal;
	makeAllFastNoiseValuesPositive(popMap, highestVal);

	/************************
		Water-boundary map
	*************************/
	std::cout << "Loading water-boundary map." << std::endl;
	WaterData waterData = WaterData(winSize);
	LoadWaterData(waterData);

	// To display
	sf::Texture waterMapTexture;
	waterMapTexture.loadFromFile("watermap.bmp");
	waterMapTexture.setSmooth(true);
	sf::Sprite waterMapSprite;
	waterMapSprite.setTexture(waterMapTexture);

	/***************************
		Quad tree
	**************************/
	std::cout << "Creating quad-tree." << std::endl;
	QuadTree* qt = new QuadTree(0, 0, winSize, winSize, nullptr, popMap, waterData, winSize, highestVal);

	/************************
			MST
	*************************/
	std::cout << "Creating MST..." << std::endl;

	MinimumSpanningTree mst = MinimumSpanningTree();
	for (auto quad : qt->GetTreeChildren())
	{
		mst.SpawnPoint(waterData, offsetForRoadNodes, quad->xOrigin, quad->yOrigin, (quad->xOrigin + quad->width), (quad->yOrigin + quad->height));
	}
	float maximumDistanceBetweenNeighbours = winSize / 4;
	mst.AssignNighbours(maximumDistanceBetweenNeighbours); // Assign neighbours
	mst.CreateAllEdges(); // Create edges
	mst.Sort(); // Sort the MST

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
	roadNodes = ds.CreatePointsAndPassBackRoadNodes(offsetForRoadNodes, waterData);

	/***************************************************
		Pathfind roads using the mst edges
		and roadnodes generated from the diamondsquare
		terrain
	****************************************************/
	std::vector<Road*> roads;
	std::cout << "Pathfinding " << mst.GetTreeEdges().size() << " roads..." << std::endl;
	int counter = 0;
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

		road->GenerateBuildingLots();

		roads.push_back(road);

		std::cout << counter++ << "/" << mst.GetTreeEdges().size() << " roads complete." << std::endl;
	}

	/*******************************************************
		Check for and remove any building lot collisions
	********************************************************/

	std::cout << "Checking for lot interceptions" << std::endl;

	int roadCounter = 0;
	// For every lot
	for (auto &road : roads) { 
		std::cout << "Checking road " << roadCounter++ << std::endl;
		for (auto &lot : road->lots) {

			// For every other lot
			for (auto &otherRoad : roads) {
				for (auto &otherLot : road->lots) {

					// Don't check a lot against itself
					if (*lot == *otherLot) { continue; }

					// We don't need to check lots that have already been checked
					if (otherLot->markForDeletion) { continue; }

					// We don't need to check lots that aren't within the minimum range
					if (V2::DistanceBetween(*lot->bottomLeft, *otherLot->bottomLeft) > 10) { continue; }

					if (lot->IsLotWithin(otherLot))
					{
						std::cout << "Setting a lot to be marked." << std::endl;
						otherLot->markForDeletion = true;
					}
				}
			}
		}
	}

	int deletionCounter = 0;
	int lotCounter = 0;

	for (auto &road : roads)
	{
		std::vector<BuildingLot*> lots = road->lots;

		for (std::vector<BuildingLot*>::iterator it = lots.begin();  it != lots.end(); /**/)
		{
			lotCounter++;
			if ((*it)->markForDeletion)
			{
				it = lots.erase(it);
				deletionCounter++;
			}	
			else
				++it;
		}
		road->lots = lots;
	}

	std::cout << lotCounter << " building lots." << std::endl;
	std::cout << deletionCounter << " deleted." << std::endl;

	/* Create window */
	sf::RenderWindow window(sf::VideoMode(winSize, winSize), "Window");

	bool drawPopMap = false,
		drawHeightMap = false,
		drawWaterBoundaryMap = false,
		drawQuadTree = false,
		drawMstNodes = false,
		drawRoads = true,
		drawMST = false,
		drawBuildingLots = false;

	std::cout << std::endl << "Instructions: " << std::endl;
	std::cout << "\t1: Toggle population map" << std::endl;
	std::cout << "\t2: Toggle height map" << std::endl;
	std::cout << "\t3: Toggle water-boundary map" << std::endl;
	std::cout << "\t4: Toggle quad-tree" << std::endl;
	std::cout << "\t5: Toggle mst nodes" << std::endl;
	std::cout << "\t6: Toggle roads" << std::endl;
	std::cout << "\t7: Toggle MST" << std::endl;
	std::cout << "\t8: Toggle building lots" << std::endl;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) { window.close(); }

			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) { drawPopMap = !drawPopMap; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) { drawHeightMap = !drawHeightMap; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) { drawWaterBoundaryMap = !drawWaterBoundaryMap; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) { drawQuadTree = !drawQuadTree; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5)) { drawMstNodes = !drawMstNodes; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6)) { drawRoads = !drawRoads; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7)) { drawMST = !drawMST; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8)) { drawBuildingLots = !drawBuildingLots; }
			}
		}

		window.clear();

		/* Population map */
		if (drawPopMap)
		{
			int numOfPoints = winSize*winSize;
			sf::VertexArray popPoints(sf::Points, numOfPoints);
			int counter = 0;
			int x = 0, y = 0;
			for (auto vec : popMap)
			{
				for (auto val : vec)
				{
					// Position
					popPoints[counter].position = sf::Vector2f(x, y);

					// Color
					sf::Color pointColour = sf::Color::White;
					float total = highestVal;
					float current = val;

					// How much percentage of the highest value is the current value
					float percentage = (current * 100) / total;

					// What is the alpha value of a single percent
					float singlePercentColour = 255 / 100;

					// How much must we therefore fill in this pixel for
					float fillAmount = singlePercentColour * percentage;

					pointColour.a = fillAmount;
					popPoints[counter].color = pointColour;

					counter++;
					x++;
				}
				y++;
				x = 0;
			}
			window.draw(popPoints);
		}
		
		/* Terrain (Heightmap) */
		if (drawHeightMap)
		{
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
		}

		/* Water boundary map */
		if (drawWaterBoundaryMap)
		{
			window.draw(waterMapSprite);
		}

		/* Quad tree */
		if (drawQuadTree)
		{
			for (auto quad : qt->GetHead()->GetTreeChildren())
			{
				sf::Vertex vertices[5] =
				{
					sf::Vertex(sf::Vector2f(quad->xOrigin, quad->yOrigin), sf::Color::Red),
					sf::Vertex(sf::Vector2f(quad->xOrigin, quad->yOrigin + quad->height), sf::Color::Red),
					sf::Vertex(sf::Vector2f(quad->xOrigin + quad->width, quad->yOrigin + quad->height), sf::Color::Red),
					sf::Vertex(sf::Vector2f(quad->xOrigin + quad->width, quad->yOrigin), sf::Color::Red),
					sf::Vertex(sf::Vector2f(quad->xOrigin, quad->yOrigin), sf::Color::Red)
				};

				window.draw(vertices, 5, sf::LineStrip);
			}
		}

		/* Roads */
		if (drawRoads)
		{
			for (Road* road : roads)
			{
				// Draw the road
				sf::VertexArray roadVertices(sf::LineStrip, road->nodes.size());
				int nodeCounter = 0;
				for (RoadNode* node : road->nodes)
				{
					roadVertices[nodeCounter].position = sf::Vector2f(node->position->x, node->position->y);
					roadVertices[nodeCounter].color = sf::Color(255, 0, 255);
					nodeCounter++;
				}
				window.draw(roadVertices);

				// Draw building lots
				if (road->lots.size() > 0)
				{
					if (drawBuildingLots)
					{
						for (BuildingLot* lot : road->lots)
						{
							sf::Vertex vertices[5] =
							{
								sf::Vertex(sf::Vector2f(lot->bottomLeft->x, lot->bottomLeft->y), sf::Color::White),
								sf::Vertex(sf::Vector2f(lot->topLeft->x, lot->topLeft->y), sf::Color::White),
								sf::Vertex(sf::Vector2f(lot->topRight->x, lot->topRight->y), sf::Color::White),
								sf::Vertex(sf::Vector2f(lot->bottomRight->x, lot->bottomRight->y), sf::Color::White),
								sf::Vertex(sf::Vector2f(lot->bottomLeft->x, lot->bottomLeft->y), sf::Color::White)
							};

							window.draw(vertices, 5, sf::LineStrip);
						}
					}
				}
			}
		}

		/* MST */
		if (drawMST)
		{
			for (Edge edge : mst.GetTreeEdges())
			{
				sf::Vertex roadVerts[2] =
				{
					sf::Vertex(sf::Vector2f(edge.start->position->x, edge.start->position->y), sf::Color::Blue),
					sf::Vertex(sf::Vector2f(edge.end->position->x, edge.end->position->y), sf::Color::Blue)
				};

				window.draw(roadVerts, 2, sf::Lines);
			}
		}


		/* Points */
		if (drawMstNodes)
		{
			for (auto node : mst.GetNodes())
			{
				sf::CircleShape shape;
				shape.setPosition(node->position->x, node->position->y);
				shape.setFillColor(sf::Color::Green);
				shape.setRadius(1);

				window.draw(shape);
			}
		}

		window.display();
	}

	return 0;
}