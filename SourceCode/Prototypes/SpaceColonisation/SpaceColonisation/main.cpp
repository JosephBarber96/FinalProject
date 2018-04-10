#include <iostream>
#include <string>
#include <vector>

#include <SFML\Graphics.hpp>

#include "Node.h"
#include "Vec2.h"
#include "UtilRandom.h"
#include "Edge.h"
#include "Road.h"
#include "RoadNetwork.h"
#include "QuadTree.h"

int winX, winY;
bool drawPopulationMap = true;
bool drawQuadTree = true;
std::vector<Node*> mainRoad = std::vector<Node*>();

Road* ConstructMainRoad()
{
	Road* mainRoad = new Road();

	// Construct the points of the main road
	mainRoad->AddPoint(new Node(winX / 2, winY - 5));
	mainRoad->AddPoint(new Node(winX / 2 + 5, winY / 2 + winY / 4));
	mainRoad->AddPoint(new Node(winX / 2, winY / 2));
	mainRoad->AddPoint(new Node(winX / 2 - 5, winY / 2 - winY / 4));
	mainRoad->AddPoint(new Node(winX / 2, 0 + 5));

	// Create edges from the main road


	// Put all points but the start and end into our startingNodes
	std::vector<Node*> startingNodes;
	startingNodes.push_back(mainRoad->getPoints()[1]);
	startingNodes.push_back(mainRoad->getPoints()[2]);
	startingNodes.push_back(mainRoad->getPoints()[3]);

	// The start and end are set to true
	mainRoad->getPoints()[0]->start = true;
	mainRoad->getPoints()[4]->start = true;

	return mainRoad;
}

int main(int argc, char* argv[])
{
	// Read in population data
	std::string populationMapFileName = "Populationmap.bmp";
	sf::Image populationMap;
	if (!populationMap.loadFromFile(populationMapFileName))
	{
		std::cerr << "Could not find / open population map" << std::endl;
		return -1;
	}

	sf::Sprite populationSprite;
	sf::Texture populationTexture;
	populationTexture.loadFromFile(populationMapFileName);
	populationTexture.setSmooth(true);
	populationSprite.setTexture(populationTexture);

	// SFML Window
	winX = populationMap.getSize().x;
	winY = populationMap.getSize().y;
	sf::RenderWindow window(sf::VideoMode(winX, winY), "Space colonisation algorithm");

	// Quad tree
	QuadTree* qt = new QuadTree(0, 0, winX, winY, nullptr, populationMap);

	// Setting up the main road
	Road* mainRoad = ConstructMainRoad();

	// Road network
	RoadNetwork network = RoadNetwork(mainRoad);

	// Create nodes
	float bufferSize = 5.f;
	for (QuadTree* child : qt->Children)
	{
		int x = UtilRandom::Instance()->Random(child->xOrigin + bufferSize, (child->xOrigin + child->width) - bufferSize);
		int y = UtilRandom::Instance()->Random(child->yOrigin + bufferSize, (child->yOrigin + child->height) - bufferSize);

		network.AddNode(new Node(x, y));
	}

	// Generate 
	network.Generate();

	// Connect orphans
	bool allConnected = false;
	do
	{
		allConnected = true;

		for (auto node : network.getNodes())
		{
			if (node->start) { continue; }

			if (node->parent == nullptr) { 
				allConnected = false; }
		} 

		if (!allConnected) {
			network.ChildOrphanNodes(); }

	} while (!allConnected);


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
				}
			}

			// Clear
			window.clear();

			// Draw...

			// Draw the population map
			if (drawPopulationMap)
			{
				window.draw(populationSprite);
			}

			// Draw the quad tree
			if (drawQuadTree)
			{
				for (QuadTree* quad : qt->Children)
				{
					sf::Vertex qtVertices[5] =
					{
						sf::Vertex(sf::Vector2f(quad->xOrigin, quad->yOrigin), sf::Color::Red),
						sf::Vertex(sf::Vector2f(quad->xOrigin, quad->yOrigin + quad->height), sf::Color::Red),
						sf::Vertex(sf::Vector2f(quad->xOrigin + quad->width, quad->yOrigin + quad->height), sf::Color::Red),
						sf::Vertex(sf::Vector2f(quad->xOrigin + quad->width, quad->yOrigin), sf::Color::Red),
						sf::Vertex(sf::Vector2f(quad->xOrigin, quad->yOrigin), sf::Color::Red)
					};

					window.draw(qtVertices, 5, sf::LineStrip);
				}
			}

			// Draw the roads
			for (auto edge : network.getEdges())
			{
				sf::Vertex roadVerts[2] =
				{
					sf::Vertex(sf::Vector2f(edge->child->getPos()->getX(), edge->child->getPos()->getY()), sf::Color::Blue),
					sf::Vertex(sf::Vector2f(edge->parent->getPos()->getX(), edge->parent->getPos()->getY()), sf::Color::Blue)
				};

				window.draw(roadVerts, 2, sf::Lines);
			}

			// Draw the major road
			sf::VertexArray mainRoadVertices(sf::LineStrip, mainRoad->getPoints().size());
			int nodeCounter = 0;
			for (auto node : mainRoad->getPoints())
			{
				mainRoadVertices[nodeCounter].position = sf::Vector2f(node->getPos()->getX(), node->getPos()->getY());
				mainRoadVertices[nodeCounter].color = sf::Color(255, 0, 255);
				nodeCounter++;
			}
			window.draw(mainRoadVertices);

			// Draw the nodes
			for (auto node : network.getNodes())
			{
				sf::Vertex point[1] = { sf::Vertex(sf::Vector2f(node->getPos()->getX(), node->getPos()->getY()), sf::Color::Green) };
				window.draw(point, 1, sf::Points);
			}


			// Display
			window.display();
		}
	}

	return 0;
}