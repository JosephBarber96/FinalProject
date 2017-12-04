#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <SFML\Graphics.hpp>

#include "Circle.h"
#include "Vec2.h"
#include "Road.h"
#include "BoundingBox.h"

int winSize = 700;

int main()
{
	// Bouding box will have a 20% boundary
	float bbMargin = winSize / 10;
	float bbX = 0 + bbMargin;
	float bbW = winSize - (2 * bbMargin);
	float bbY = 0 + bbMargin;
	float bbH = winSize - (2 * bbMargin);
	BoundingBox bb = BoundingBox(bbX-1, bbW+1, bbY+2, bbH-2);

	/*
		Roads to represent the bb
		(slightly offset as the intersection checking doesn't handle perfectly
		vertical lines)
	*/
	std::vector<Road> bbRoads;
	{
		// top left
		Vec2 topLeft = Vec2(bb.xOrigin + 1, bb.yOrigin - 1);
		// top right
		Vec2 topRight = Vec2(bb.xOrigin + bb.width - 1, bb.yOrigin);
		// bottom right
		Vec2 bottomRight = Vec2(bb.xOrigin + bb.width + 1, bb.yOrigin - 1 + bb.height);
		// bottom left
		Vec2 bottomLeft = Vec2(bb.xOrigin - 1, bb.yOrigin + 1 + bb.height + 1);

		// top left -> top right
		bbRoads.push_back(Road(topLeft.x, topLeft.y, topRight.x, topRight.y));
		// top right -> bottom right
		bbRoads.push_back(Road(topRight.x, topRight.y, bottomRight.x, bottomRight.y));
		// bottom right -> bottom left
		bbRoads.push_back(Road(bottomRight.x, bottomRight.y, bottomLeft.x, bottomLeft.y));
		// bottom left -> top left
		bbRoads.push_back(Road(bottomLeft.x, bottomLeft.y, topLeft.x, topLeft.y));
	}



	// 1 dial will occur every x meters
	int metersPerCircle = 90;
	int numOfCircles = winSize / metersPerCircle;
	std::vector<Circle> dials;
	int circlePoints = 100;
	for (int i = 0; i < numOfCircles; i++)
	{
		Circle circle;

		// Create the points (normalized) for the circle
		// Refference: https://www.opengl.org/discussion_boards/showthread.php/167955-drawing-a-smooth-circle
		for (int j = 0; j < circlePoints; j++)
		{
			double angle = 2 * M_PI * j / circlePoints;

			double x = cos(angle);
			double y = sin(angle);

			circle.AddPoint(x, y);
		}
		dials.push_back(circle);
	}

	/*
		Space these out evenly, whilst also converting
		into SFML screen space
	*/
	int counter = 1;
	for (Circle &cir : dials)
	{
		for (Vec2* v : cir.points)
		{
			v->x *= (counter * metersPerCircle);
			v->y *= (counter * metersPerCircle);

			v->x += winSize / 2;
			v->y += winSize / 2;
		}
		counter++;
	}

	// Create roads between these points
	std::vector<Road> roads;
	for (const Circle &cir : dials)
	{
		for (int i = 0; i < circlePoints - 1; i++)
		{
			Road road;
			road.start = cir.points[i];

			if (i < circlePoints - 2)
			{
				road.end = cir.points[i+1];
			}
			else
			{
				road.end = cir.points[0];
			}

			roads.push_back(road);
		}
	}


	/*
		Remove any points or roads that do not 
		fit within our bounding box
	*/
	// points
	for (Circle &cir : dials)
	{
		for (std::vector<Vec2*>::iterator iter = cir.points.begin(); iter != cir.points.end(); /*no incrementing in loop*/)
		{
			// If this point is not within the bounding box
			if (!bb.isWithin((*iter)))
			{
				// Remove it
				iter = cir.points.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
	// roads
	for (std::vector<Road>::iterator iter = roads.begin(); iter != roads.end(); /* no incrementing in loop */)
	{
		// If one of the roads points is outside of the bounding box
		if (!bb.isWithin((*iter).start) || !bb.isWithin((*iter).end))
		{
			// Remove it
			iter = roads.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	/*
		Create roads going outwards
	*/
	int numOfOutwardsRoads = 7;
	std::vector<Road> outwardsRoads;
	for (int i = 0; i < numOfOutwardsRoads; i++)
	{
		double angle = 2 * M_PI * i / numOfOutwardsRoads;

		double x = cos(angle);
		double y = sin(angle);

		Vec2 normalizedPos = Vec2(x, y);

		// Start = normalizedPos converted into sfml space
		float sX = normalizedPos.x;
		float sY = normalizedPos.y;

		sX += winSize / 2;
		sY += winSize / 2;

		// End = normalizedPos scaled by 10 converted into sfml space
		float eX = normalizedPos.x * 20;
		float eY = normalizedPos.y * 20;

		eX += winSize / 2;
		eY += winSize / 2;

		outwardsRoads.push_back(Road(sX, sY, eX, eY));
	}

	/*
		Extend these roads until they colide with the bounding Box
	*/
	for (Road &road : outwardsRoads)
	{
		road.ExtendUntilHit(bbRoads);
	}

	sf::RenderWindow window(sf::VideoMode(winSize, winSize), "Radial template");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) { window.close(); }
		}

		// Clear
		window.clear();

		// Draw

		// Points
		//for (const Circle &circle : dials)
		//{
		//	for (const Vec2* vec : circle.points)
		//	{
		//		sf::CircleShape shape;
		//		shape.setPosition(vec->x, vec->y);
		//		shape.setRadius(1);
		//		shape.setFillColor(sf::Color::Yellow);

		//		window.draw(shape);
		//	}
		//}


		// Roads
		for (const Road &road : roads)
		{
			const int sz = 2;
			sf::Vertex roadPoints[2] =
			{
				sf::Vertex(sf::Vector2f(road.start->x, road.start->y), sf::Color::Blue),
				sf::Vertex(sf::Vector2f(road.end->x, road.end->y), sf::Color::Blue)
			};

			window.draw(roadPoints, sz, sf::LineStrip);
		}

		for (const Road &road : outwardsRoads)
		{
			const int sz = 2;
			sf::Vertex roadPoints[2] =
			{
				sf::Vertex(sf::Vector2f(road.start->x, road.start->y), sf::Color::Yellow),
				sf::Vertex(sf::Vector2f(road.end->x, road.end->y), sf::Color::Yellow)
			};

			window.draw(roadPoints, sz, sf::LineStrip);
		}

		// Bounding box
		const int sz = 5;
		sf::Vertex boundingBoxPoint[sz] =
		{
			// Top left
			sf::Vertex(sf::Vector2f(bb.xOrigin, bb.yOrigin), sf::Color::Green),
			// Top right
			sf::Vertex(sf::Vector2f(bb.xOrigin + bb.width, bb.yOrigin), sf::Color::Green),
			// Bottom right
			sf::Vertex(sf::Vector2f(bb.xOrigin + bb.width, bb.yOrigin + bb.height), sf::Color::Green),
			// Bottom left
			sf::Vertex(sf::Vector2f(bb.xOrigin, bb.yOrigin + bb.height), sf::Color::Green),
			// Top left again to finish off the line strip
			sf::Vertex(sf::Vector2f(bb.xOrigin, bb.yOrigin), sf::Color::Green)
		};

		window.draw(boundingBoxPoint, sz, sf::LineStrip);

		// Display
		window.display();
	}
	return 0;
}