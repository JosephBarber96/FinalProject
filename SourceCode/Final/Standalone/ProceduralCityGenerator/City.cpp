#include <iostream>

#include <SFML\Graphics.hpp>

#include "City.h"

#include "UtilRandom.h"
#include "Utility.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Pathfinding.h"

#include "PopulationMap.h"
#include "DiamondSquare.h"
#include "WaterData.h"
#include "PopulationQuadTree.h"
#include "RoadNetwork.h"
#include "MinorRoad.h"
#include "MajorRoad.h"
#include "MinimumSpanningTree.h"
#include "RoadNode.h"
#include "BuildingLot.h"
#include "MstEdge.h"
#include "MstNode.h"
#include "FloorPlan.h"
#include "shapeLine.h"


City::City() {}

City::~City() {}

void City::Generate()
{
	/* Population map */
	GeneratePopulationMap();

	/* Terrain */
	GenerateTerrain();

	/* Water boundary map */
	GenerateWaterBoundary();

	/* Quad tree */
	GenerateQuadTree();

	/* Voronoi */
	GenerateVoronoi();

	/* Minor roads */
	GenerateMinorRoads();

	/* Mst */
	GenerateMST();

	/* Road nodes */
	GenerateRoadNodes();

	/* Major roads */
	GenerateMajorRoads();

	/* Validate roads */
	ValidateRoads();

	/* Building lots */
	GenerateBuildingLots();

	/* Buildings */
	GenerateBuildings();
}

void City::Draw()
{
	/* Create window */
	sf::RenderWindow window(sf::VideoMode(winSize, winSize), "Window");
	sf::View view(sf::FloatRect(0, 0, winSize, winSize));
	window.setView(view);

	bool drawPopMap = false,
		drawHeightMap = false,
		drawWaterBoundaryMap = true,
		drawQuadTree = false,
		drawMstNodes = false,
		drawRoads = true,
		drawMST = false,
		drawBuildingLots = false,
		drawBuildings = true,
		drawMinorRoads = true,
		drawVoronoiPointsForMinorRoads = false;

	std::cout << std::endl << "Instructions: " << std::endl;
	std::cout << "\t1: Toggle population map" << std::endl;
	std::cout << "\t2: Toggle height map" << std::endl;
	std::cout << "\t3: Toggle water-boundary map" << std::endl;
	std::cout << "\t4: Toggle quad-tree" << std::endl;
	std::cout << "\t5: Toggle mst nodes" << std::endl;
	std::cout << "\t6: Toggle roads" << std::endl;
	std::cout << "\t7: Toggle MST" << std::endl;
	std::cout << "\t8: Toggle minor roads" << std::endl;
	std::cout << "\t9: Toggle building lots" << std::endl;
	std::cout << "\t0: Toggle buildings" << std::endl;
	std::cout << std::endl;
	std::cout << "\t WASD - Move camera" << std::endl;
	std::cout << "\t Q/E - (Un)zoom camera" << std::endl;

	while (window.isOpen())
	{
		window.setView(view);
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
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8)) { drawMinorRoads = !drawMinorRoads; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9)) { drawBuildingLots = !drawBuildingLots; }
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0)) { drawBuildings = !drawBuildings; }

				// Camera movement
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
					view.move(0, -10);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
					view.move(-10, 0);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
					view.move(0, 10);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
					view.move(10, 0);

				// Camera zooming
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
				{
					view.zoom(1.1f);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
				{
					view.zoom(0.9);
				}
			}
		}

		window.clear();

		/* Population map */
		if (drawPopMap)
		{
			int numOfPoints = winSize * winSize;
			sf::VertexArray popPoints(sf::Points, numOfPoints);
			int counter = 0;
			int x = 0, y = 0;
			for (auto vec : populationMap->GetPopulationMap())
			{
				for (auto val : vec)
				{
					// Position
					popPoints[counter].position = sf::Vector2f(x, y);

					// Color
					sf::Color pointColour = sf::Color::White;
					float total = populationMap->GetHighestValue();
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
			int numOfVerts = terrain->Points().size() * terrain->Points()[0].size();
			sf::VertexArray vertPoints(sf::Points, numOfVerts);
			int counter = 0;
			for (auto vec : terrain->Points())
			{
				for (auto v : vec)
				{
					// Position
					vertPoints[counter].position = sf::Vector2f(v->x, v->y);

					// Color
					sf::Color shapeColor = sf::Color::White;
					int total = terrain->Highest();
					float current = v->z;
					current += (abs(terrain->Lowest()));
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
			int numOfPoints = winSize*winSize;
			sf::VertexArray waterPoints(sf::Points, numOfPoints);
			int counter = 0;
			for (int y = 0; y < winSize; y++)
			{
				for (int x = 0; x < winSize; x++)
				{
					waterPoints[counter].position = sf::Vector2f(x, y);
					waterPoints[counter].color = (waterData->IsWater(x, y)) ? sf::Color::White : sf::Color(0, 0, 0, 0);
					counter++;
				}
			}

			window.draw(waterPoints);
		}

		/* Quad tree */
		if (drawQuadTree)
		{
			for (auto quad : quadTree->TreeHead()->AllChildren())
			{
				sf::Color colour;
				switch (dynamic_cast<PopulationQuadTree*>(quad)->PopulationDensity())
				{
				case QuadPopulation::high:
					colour = sf::Color::Blue;
					break;
				case QuadPopulation::med:
					colour = sf::Color::Red;
					break;
				case QuadPopulation::low:
					colour = sf::Color::Yellow;
					break;
				}

				sf::Vertex vertices[5] =
				{
					sf::Vertex(sf::Vector2f(quad->XOrigin(), quad->YOrigin()), colour),
					sf::Vertex(sf::Vector2f(quad->XOrigin(), quad->YOrigin() + quad->Height()), colour),
					sf::Vertex(sf::Vector2f(quad->XOrigin() + quad->Width(), quad->YOrigin() + quad->Height()), colour),
					sf::Vertex(sf::Vector2f(quad->XOrigin() + quad->Width(), quad->YOrigin()), colour),
					sf::Vertex(sf::Vector2f(quad->XOrigin(), quad->YOrigin()), colour)
				};

				window.draw(vertices, 5, sf::LineStrip);
			}
		}

		/* Major roads */
		if (drawRoads)
		{
			for (MajorRoad* road : roadNetwork->majorRoads)
			{
				// Draw each segment
				for (MajorRoad* segment : road->segments)
				{
					sf::Vertex vertices[2] =
					{
						sf::Vertex(sf::Vector2f(segment->Start()->x, segment->Start()->y), sf::Color(255, 0, 255)),
						sf::Vertex(sf::Vector2f(segment->End()->x, segment->End()->y), sf::Color(255, 0, 255))
					};
					window.draw(vertices, 2, sf::LinesStrip);
				}
			}
		}

		/* Minor roads */
		if (drawMinorRoads)
		{
			for (MinorRoad* road : roadNetwork->minorRoads)
			{
				// Draw the road
				sf::VertexArray roadVertices(sf::LineStrip, 2);

				roadVertices[0].position = sf::Vector2f(road->Start()->x, road->Start()->y);
				roadVertices[0].color = sf::Color(255, 0, 0, 100);

				roadVertices[1].position = sf::Vector2f(road->End()->x, road->End()->y);
				roadVertices[1].color = sf::Color(255, 0, 0, 100);

				window.draw(roadVertices);

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
			for (MstEdge* edge : mst->GetTreeEdges())
			{
				sf::Vertex roadVerts[2] =
				{
					sf::Vertex(sf::Vector2f(edge->start->position->x, edge->start->position->y), sf::Color::Blue),
					sf::Vertex(sf::Vector2f(edge->end->position->x, edge->end->position->y), sf::Color::Blue)
				};

				window.draw(roadVerts, 2, sf::Lines);
			}
		}

		/* Points */
		if (drawMstNodes)
		{
			for (auto node : mst->GetNodes())
			{
				sf::CircleShape shape;
				shape.setPosition(node->position->x, node->position->y);
				shape.setFillColor(sf::Color::Green);
				shape.setRadius(1);

				window.draw(shape);
			}
		}

		/* Floor plans, buildings */
		if (drawBuildings)
		{
			for (MinorRoad* road : roadNetwork->minorRoads)
			{
				for (BuildingLot* lot : road->lots)
				{
					// lot->fp->bb->DrawSelf(&window);
					lot->fp->DrawPerimeter(&window);
				}
			}
		}

		window.display();
	}
}

/* Private methods */

void City::GeneratePopulationMap()
{
	std::cout << "Generating population map." << std::endl;
	populationMap = new PopulationMap();
	populationMap->Generate(winSize);
}

void City::GenerateTerrain()
{
	std::cout << "Generating terrain." << std::endl;
	int divisions = winSize;
	int size = 5;
	int height = 64;
	terrain = new DiamondSquare(divisions, size, height);
	terrain->Generate();
	terrain->CreatePoints();
	terrain->CalcuateBoundaryPoints();
}

void City::GenerateWaterBoundary()
{
	std::cout << "Generating water." << std::endl;
	waterData = new WaterData(winSize);
	waterData->LoadFromTerrain(terrain, 10);
}

void City::GenerateQuadTree()
{
	std::cout << "Creating QuadTree." << std::endl;
	quadTree = new PopulationQuadTree(0, 0, winSize, winSize, nullptr, populationMap, waterData, winSize);
}

void City::GenerateVoronoi()
{
	std::cout << "Generating voronoi." << std::endl;
	// To hold voronoiPoints
	std::vector<point_type> voronoiPoints;
	int bufferSpace = 5;

	// For each quad in the quad tree
	for (auto quad : quadTree->AllChildren())
	{
		// Get the boundaries of this quad
		int minX = quad->XOrigin();
		int maxX = quad->XOrigin() + quad->Width();
		int minY = quad->YOrigin();
		int maxY = quad->YOrigin() + quad->Height();

		// Do not place a point in a quad that contains water..

		// Find all valid land positions in this quad
		std::vector<Vec2> waterPositions;
		for (int y = minY; y < maxY; y++)
		{
			for (int x = minX; x < maxX; x++)
			{
				if (waterData->IsWater(x, y))
				{
					waterPositions.push_back(Vec2(x, y));
				}
			}
		}
		// If none exist, continue without placing a point
		if (waterPositions.size() > 0) { continue; }

		// Otherwise, place a point
		double px = UtilRandom::Instance()->RandomFloat(minX + bufferSpace, maxX - bufferSpace);
		double py = UtilRandom::Instance()->RandomFloat(minY + bufferSpace, maxY - bufferSpace);

		voronoiPoints.push_back(point_type(px, py));
	}

	// Construct the voronoi diagram
	construct_voronoi(voronoiPoints.begin(), voronoiPoints.end(), &voronoi);
}

void City::GenerateMinorRoads()
{
	std::cout << "Creating minor roads." << std::endl;
	roadNetwork = new RoadNetwork();
	roadNetwork->GenerateMinorRoads(&voronoi);

	std::cout << "Minor roads created: " << roadNetwork->minorRoads.size() << std::endl;
}

void City::GenerateMST()
{
	std::cout << "Generating Minimum spanning tree." << std::endl;
	mst = new MinimumSpanningTree();

	// Spawn a point in each quad
	for (auto quad : quadTree->AllChildren())
	{
		if (static_cast<PopulationQuadTree*>(quad)->Indexer() > 2)
		{
			mst->SpawnPoint(waterData, 
				offsetForRoadNodes, 
				quad->XOrigin(), 
				quad->YOrigin(), 
				(quad->XOrigin() + quad->Width()), 
				(quad->YOrigin() + quad->Height()));
		}
	}

	mst->AssignNeighbours(winSize / 4);
	mst->CreateEdges();
	mst->Sort();
}

void City::GenerateRoadNodes()
{
	std::cout << "Generating road nodes." << std::endl;
	roadNetwork->roadNodes = terrain->CreatePointsAndPassBackRoadNodes(offsetForRoadNodes, waterData);
}

void City::GenerateMajorRoads()
{
	std::cout << "Generating major roads." << std::endl;
	roadNetwork->GenerateMajorRoads(mst, offsetForRoadNodes);
}

void City::ValidateRoads()
{
	std::cout << "Validating roads." << std::endl;
	roadNetwork->ValidateRoads(waterData);
}

void City::GenerateBuildingLots()
{
	std::cout << "Generating building lots." << std::endl;
	roadNetwork->GenerateBuildingLots();
	roadNetwork->ValidateBuildingLots(waterData);
}

void City::GenerateBuildings()
{
	std::vector<Vec2*> lotCenter;
	std::vector<Vec2*> buildingCenter;

	for (auto road : roadNetwork->minorRoads)
	{
		for (auto lot : road->lots)
		{
			// Give the lot a new FloorPlan object
			lot->fp = new FloorPlan();

			// Create the lots bounding box for building generation
			lot->fp->SetBoundingBox(lot->minX, lot->minY, lot->maxX - lot->minX, lot->maxY - lot->minY);

			// Generate shapes
			int numberOfShapes = UtilRandom::Instance()->RandomInt(2, 5);
			lot->fp->GenerateShapes(numberOfShapes);

			// Calculate building perimeter
			lot->fp->GeneratePerimeter();

			// Find the center point of the generated building

			// First we need to find the minimum and maximum boundaries
			float buildingMinX = FLT_MAX, buildingMaxX = -FLT_MAX, buildingMinY = FLT_MAX, buildingMaxY = -FLT_MAX;
			for (shapeLine* line : lot->fp->perimeterLines)
			{
				if (line->start->x < buildingMinX) buildingMinX = line->start->x;
				if (line->start->x > buildingMaxX) buildingMaxX = line->start->x;

				if (line->start->y < buildingMinY) buildingMinY = line->start->y;
				if (line->start->y > buildingMaxY) buildingMaxY = line->start->y;

				if (line->end->x < buildingMinX) buildingMinX = line->start->x;
				if (line->end->x > buildingMaxX) buildingMaxX = line->start->x;

				if (line->end->y < buildingMinY) buildingMinY = line->start->y;
				if (line->end->y > buildingMaxY) buildingMaxY = line->start->y;
			}

			// Then we find the center
			float buildingDiffX = buildingMaxX - buildingMinX;
			float buildingDiffY = buildingMaxY - buildingMinY;

			float buildingMidX = buildingMaxX - (buildingDiffX / 2);
			float buildingMidY = buildingMaxY - (buildingDiffY / 2);


			/*
			Find the angle to rotate by
			*/

			// Find the roads facing angle
			Vec2 parentDir = Vec2(
				road->End()->x - road->Start()->x,
				road->End()->y - road->Start()->y);
			float parentAngle = Vec2::VectorToAngle(parentDir);

			float angleToRotate = 90 - parentAngle - 45;

			// Rotate the points around the found center point
			for (shapeLine* line : lot->fp->perimeterLines)
			{
				line->start = Utility::RotateAroundPoint(line->start, new Vec2(buildingMidX, buildingMidY), angleToRotate);
				line->end = Utility::RotateAroundPoint(line->end, new Vec2(buildingMidX, buildingMidY), angleToRotate);
			}

			/*
			Move the building so the center point of the building is the center point of the shape
			*/

			// Find the center of the lot
			float lotDiffX = lot->maxX - lot->minX;
			float lotDiffY = lot->maxY - lot->minY;

			float lotMidX = lot->maxX - (lotDiffX / 2);
			float lotMidY = lot->maxY - (lotDiffY / 2);

			// Find out the offset
			float offsetX = lotMidX - buildingMidX;
			float offsetY = lotMidY - buildingMidY;

			buildingCenter.push_back(new Vec2(buildingMidX, buildingMidY));
			lotCenter.push_back(new Vec2(lotMidX, lotMidY));

			// Move the lines by this much
			for (shapeLine* line : lot->fp->perimeterLines)
			{
				line->start->x += offsetX;
				line->start->y += offsetY;

				line->end->x += offsetX;
				line->end->y += offsetY;
			}
		}
	}
}