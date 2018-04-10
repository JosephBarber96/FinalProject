#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <algorithm>

/*
Requires C++ Boost Library for 1.65s for windows
https://dl.bintray.com/boostorg/release/1.65.1/binaries/
(boost_1_65_1-msvc-10.0-32.exe)

include directory: C:\local\boost_1_65_1
*/
#include<boost\polygon\voronoi.hpp>
#include <SFML\Graphics.hpp>

#include "Circle.h"
#include "Vec2.h"
#include "Road.h"
#include "BoundingBox.h"
#include "Box.h"
#include "UtilRandom.h"
#include "MinorRoad.h"

using namespace boost::polygon;

typedef int coordinate_type;
typedef point_data<coordinate_type> point_type;
typedef voronoi_diagram<double> VD;

int winSize = 700;

void CreateRasterMinorRoads(std::vector<Box> &boxes, std::vector<Road> &allMajorRoads);

int main()
{
	// Bouding box will have a boundary
	float bbMargin = winSize / 18;
	float bbX = 0 + bbMargin;
	float bbW = winSize - (2 * bbMargin);
	float bbY = 0 + bbMargin;
	float bbH = winSize - (2 * bbMargin);
	BoundingBox bb = BoundingBox(bbX - 1, bbW + 1, bbY + 2, bbH - 2);

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
	int metersPerCircle = 70;
	int numOfCircles = winSize / metersPerCircle;
	std::vector<Circle> dials;
	int circlePoints = 80;
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
	std::vector<Road> circleRoads;
	for (const Circle &cir : dials)
	{
		for (int i = 0; i < circlePoints - 1; i++)
		{
			Road road;
			road.start = cir.points[i];

			if (i < circlePoints - 2)
			{
				road.end = cir.points[i + 1];
			}
			else
			{
				road.end = cir.points[0];
			}

			circleRoads.push_back(road);
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
	for (std::vector<Road>::iterator iter = circleRoads.begin(); iter != circleRoads.end(); /* no incrementing in loop */)
	{
		// If one of the roads points is outside of the bounding box
		if (!bb.isWithin((*iter).start) || !bb.isWithin((*iter).end))
		{
			// Remove it
			iter = circleRoads.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	/*
		Create roads going outwards
	*/
	int numOfOutwardsRoads = 9;
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


	/*
		Find the points of intersection for each outward road
	*/

	/*
		They will be stored in a 2D container, in order to store them
		in "layers" (index 0 = first circle dial, index = 1 second, etc).
	*/
	std::vector<std::vector<Vec2*>> intersectionPoints;

	auto comparePoints = [](Vec2* lhs, Vec2* rhs) -> bool
	{
		return (lhs->x == rhs->x && lhs->y == rhs->y);
	};

	// For each road
	for (Road road : outwardsRoads)
	{
		// Get all of the intersection points for this road
		std::vector<Vec2*> iPoints = road.GetAllIntersectionPoints(circleRoads);

		// Delete any duplicate points for intersection
		iPoints.erase(unique(iPoints.begin(), iPoints.end(), comparePoints), iPoints.end());

		// counter keeps track of the layer
		int layer = 0;

		// Add each point to a new layer
		for (Vec2* ip : iPoints)
		{
			// Add layers as neccessary
			if (intersectionPoints.size() <= layer)
			{
				intersectionPoints.push_back(std::vector<Vec2*>());
			}

			// Add this point to the correct layer
			intersectionPoints[layer].push_back(ip);

			// layer++
			layer++;
		}
	}

	/*
		Draw boxes between each of these points
	*/

	std::vector<Box> boxes;
	int intersectionLayers = intersectionPoints.size();
	int outRoads = intersectionPoints[1].size();

	for (int layer = 0; layer < intersectionLayers; layer++)
	{
		for (int road = 0; road < outRoads; road++)
		{
			// If we're on the final layer of intersections, break.
			if (layer > 0)
			{
				if (intersectionPoints[layer].size() < intersectionPoints[layer - 1].size()) { break; }
			}
			if (layer < intersectionLayers - 1)
			{
				if (intersectionPoints[layer].size() > intersectionPoints[layer + 1].size()) { break; }
			}
			
			
			/*
				[layer] [road]

				[0][1] = first layer, 2nd road
				[1][1] = first layer, 2nd road
			*/
			Box box;

			/*
				Our current layer, current road
			*/
			Vec2* bottomLeft = intersectionPoints[layer][road];

			
			/***********************
				Layer +1, same road
			************************/
			Vec2* topLeft = intersectionPoints[layer + 1][road];

			/*********************
				Layer +1, road +1
			**********************/
			Vec2* topRight;

			// Wrap around?
			if (road + 1 == outRoads)
			{
				topRight = intersectionPoints[layer + 1][0];
			}
			else
			{
				topRight = intersectionPoints[layer + 1][road + 1];
			}

			/***********************
				same layer, road +1
			************************/
			Vec2* bottomRight;

			// Wrap around?
			if (road + 1 == outRoads)
			{
				bottomRight = intersectionPoints[layer][0];
			}
			else
			{
				bottomRight = intersectionPoints[layer][road + 1];
			}

			box = Box(topLeft, topRight, bottomLeft, bottomRight);

			boxes.push_back(box);
		}
	}

	// Append all of the roads into one vector;
	std::vector<Road> allMajorRoads;
	allMajorRoads.insert(allMajorRoads.end(), circleRoads.begin(), circleRoads.end());
	allMajorRoads.insert(allMajorRoads.end(), outwardsRoads.begin(), outwardsRoads.end());

	// CreateRasterMinorRoads(boxes, allMajorRoads);

	/*
		Create voronoi minor roads
	*/

	// Points

	// for each box...
	int pointsPerBox = 17;
	std::vector<Road> voronoiRoads;
	for (const auto &box : boxes)
	{
		// Find the edge points of the box
		Vec2* leftMost, *rightMost, *topMost, *bottomMost;

		// leftMost
		leftMost = box.bottomLeft;
		if (box.topLeft->x < leftMost->x) { leftMost = box.topLeft; }
		if (box.topRight->x < leftMost->x) { leftMost = box.topRight; }
		if (box.bottomRight->x < leftMost->x) { leftMost = box.bottomRight; }

		// rightMost
		rightMost = box.bottomLeft;
		if (box.topLeft->x > rightMost->x) { rightMost = box.topLeft; }
		if (box.topRight->x > rightMost->x) { rightMost = box.topRight; }
		if (box.bottomRight->x > rightMost->x) { rightMost = box.bottomRight; }

		// topMost
		topMost = box.bottomLeft;
		if (box.topLeft->y > topMost->y) { topMost = box.topLeft; }
		if (box.topRight->y > topMost->y) { topMost = box.topRight; }
		if (box.bottomRight->y > topMost->y) { topMost = box.bottomRight; }

		// bottomMost
		bottomMost = box.bottomLeft;
		if (box.topLeft->y < bottomMost->y) { bottomMost = box.topLeft; }
		if (box.topRight->y < bottomMost->y) { bottomMost = box.topRight; }
		if (box.bottomRight->y < bottomMost->y) { bottomMost = box.bottomRight; }

		int minX = leftMost->x;
		//minX = minX - minX;
		int maxX = rightMost->x;
		//maxX = maxX + maxX;
		int minY = bottomMost->y;
		//minY = minY - minY;
		int maxY = topMost->y;
		//maxY = maxY + maxY;

		// Place points in the boundaries of the box
		std::vector<point_type> voronoiBoxPoints;

		for (int i = 0; i < pointsPerBox; i++)
		{
			double pointX = UtilRandom::Instance()->Random(minX, maxX);
			double pointY = UtilRandom::Instance()->Random(minY, maxY);

			voronoiBoxPoints.push_back(point_type(pointX, pointY));
		}

		// Construct the voronoi diagram
		VD vd;
		construct_voronoi(voronoiBoxPoints.begin(), voronoiBoxPoints.end(), &vd);

		// Create roads from the voronoi edges
		for (auto const &edge : vd.edges())
		{
			if (edge.vertex0() != NULL && edge.vertex1() != NULL)
			{
				int sX = edge.vertex0()->x();
				int sY = edge.vertex0()->y();

				int eX = edge.vertex1()->x();
				int eY = edge.vertex1()->y();

				Vec2* roadStart, *roadEnd;

				/* if sX and sY are in bounds, its the start */
				if (sX >= minX && sX <= maxX
					&& sY >= minY && sY <= maxY)
				{
					roadStart = new Vec2(sX, sY);
					roadEnd = new Vec2(eX, eY);
				}
				// else, end is start
				else
				{
					roadStart = new Vec2(eX, eY);
					roadEnd = new Vec2(sX, sY);
				}

				Road road = Road(roadStart, roadEnd);

				/* If this road already exists, don't push it back */
				bool exists = false;
				for (auto r : voronoiRoads)
				{
					if (*road.start == *r.end && *road.end == *r.start
						||
						*road.start == *r.start && *road.end == *r.end)
					{
						exists = true;
						break;
					}
				}
				if (!exists)
				{
					if (*road.start == *road.end)
					{

					}
					else
					{
						road.CutOffAtIntersection(allMajorRoads);
						road.CutOffAtIntersection(bbRoads);

						if (road.start->x >= minX && road.start->x <= maxX && road.start->y >= minY && road.start->y <= maxY
							&&
							road.end->x >= minX && road.end->x <= maxX && road.end->y >= minY && road.end->y <= maxY)
						{
							voronoiRoads.push_back(road);
						}



						
					}
				}

				//// Collect all points of intersection
				//std::vector<Vec2*> ips = road.GetAllIntersectionPoints(allMajorRoads);
				//vdips.insert(vdips.end(), ips.begin(), ips.end());

			}
		}
	}

	// Clip the roads after
	/*
		We clip them after because they have to remain un-modified 
		whilst we are creating all of the roads so that we can 
		check against any dupes being created.
	*/


	for (Road &road : voronoiRoads)
	{
		//road.CutOffAtIntersection(allMajorRoads);
		//road.CutOffAtIntersection(bbRoads);
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
		for (const Road &road : voronoiRoads)
		{
			const int sz = 2;
			sf::Vertex roadPoints[sz] =
			{
				sf::Vertex(sf::Vector2f(road.start->x, road.start->y), sf::Color::Blue),
				sf::Vertex(sf::Vector2f(road.end->x, road.end->y), sf::Color::Blue)
			};

			window.draw(roadPoints, sz, sf::LineStrip);
		}

		for (const Road &road : circleRoads)
		{
			const int sz = 2;
			sf::Vertex roadPoints[sz] =
			{
				sf::Vertex(sf::Vector2f(road.start->x, road.start->y), sf::Color(255, 165, 0, 255)),
				sf::Vertex(sf::Vector2f(road.end->x, road.end->y), sf::Color(255, 165, 0, 255))
			};

			window.draw(roadPoints, sz, sf::LineStrip);
		}

		for (const Road &road : outwardsRoads)
		{
			const int sz = 2;
			sf::Vertex roadPoints[sz] =
			{
				sf::Vertex(sf::Vector2f(road.start->x, road.start->y), sf::Color(255, 165, 0, 255)),
				sf::Vertex(sf::Vector2f(road.end->x, road.end->y), sf::Color(255, 165, 0, 255))
			};

			window.draw(roadPoints, sz, sf::LineStrip);
		}

		//for (const MinorRoad &road : minorRoads)
		//{
		//	const int sz = 2;
		//	sf::Vertex roadPoints[sz] =
		//	{
		//		sf::Vertex(sf::Vector2f(road.start->x, road.start->y), sf::Color(255, 165, 0, 255)),
		//		sf::Vertex(sf::Vector2f(road.end->x, road.end->y), sf::Color(255, 165, 0, 255))
		//	};

		//	window.draw(roadPoints, sz, sf::LineStrip);
		//}

		// The roads of the bounding box
		for (const Road &road : bbRoads)
		{
			const int sz = 2;
			sf::Vertex bbRoadPoints[sz] =
			{
				sf::Vertex(sf::Vector2f(road.start->x, road.start->y), sf::Color::Green),
				sf::Vertex(sf::Vector2f(road.end->x, road.end->y), sf::Color::Green)
			};

			window.draw(bbRoadPoints, sz, sf::LineStrip);
		}

		// intersectionPoints
		//for (auto ipContainer : intersectionPoints)
		//{
		//	for (Vec2* ip : ipContainer)
		//	{
		//		sf::CircleShape shape;
		//		shape.setPosition(ip->x - 1, ip->y - 1);
		//		shape.setRadius(3);
		//		shape.setFillColor(sf::Color::Red);

		//		window.draw(shape);
		//	}
		//}


		// Boxs
		//for (Box &box : boxes)
		//{
		//	sf::Vertex boxPoints[5] =
		//	{
		//		// top left
		//		sf::Vertex(sf::Vector2f(box.topLeft->x, box.topLeft->y), sf::Color(255, 0, 255, 255)),
		//		// top right
		//		sf::Vertex(sf::Vector2f(box.topRight->x, box.topRight->y), sf::Color(255, 0, 255, 255)),
		//		// bottom right
		//		sf::Vertex(sf::Vector2f(box.bottomRight->x, box.bottomRight->y), sf::Color(255, 0, 255, 255)),
		//		// bottom left
		//		sf::Vertex(sf::Vector2f(box.bottomLeft->x, box.bottomLeft->y), sf::Color(255, 0, 255, 255)),
		//		// top left again to finish off the box
		//		sf::Vertex(sf::Vector2f(box.topLeft->x, box.topLeft->y), sf::Color(255, 0, 255, 255)),
		//	};
		//	window.draw(boxPoints, 5, sf::LineStrip);
		//}

		// Center points
		//for (auto vec : centerPoints)
		//{
		//	sf::CircleShape shape;
		//	shape.setPosition(vec->x, vec->y);
		//	shape.setRadius(2);
		//	shape.setFillColor(sf::Color::Cyan);

		//	window.draw(shape);
		//}

		// Display
		window.display();
	}
	return 0;
}

void CreateRasterMinorRoads(std::vector<Box> &boxes, std::vector<Road> &allMajorRoads)
{
	/*
	Put a point in the center of each block
	*/
	std::vector<Vec2*> centerPoints;
	for (auto &box : boxes)
	{
		Vec2* leftMost, *rightMost, *topMost, *bottomMost;

		// leftMost
		leftMost = box.bottomLeft;
		if (box.topLeft->x < leftMost->x) { leftMost = box.topLeft; }
		if (box.topRight->x < leftMost->x) { leftMost = box.topRight; }
		if (box.bottomRight->x < leftMost->x) { leftMost = box.bottomRight; }

		// rightMost
		rightMost = box.bottomLeft;
		if (box.topLeft->x > rightMost->x) { rightMost = box.topLeft; }
		if (box.topRight->x > rightMost->x) { rightMost = box.topRight; }
		if (box.bottomRight->x > rightMost->x) { rightMost = box.bottomRight; }

		// topMost
		topMost = box.bottomLeft;
		if (box.topLeft->y > topMost->y) { topMost = box.topLeft; }
		if (box.topRight->y > topMost->y) { topMost = box.topRight; }
		if (box.bottomRight->y > topMost->y) { topMost = box.bottomRight; }

		// bottomMost
		bottomMost = box.bottomLeft;
		if (box.topLeft->y < bottomMost->y) { bottomMost = box.topLeft; }
		if (box.topRight->y < bottomMost->y) { bottomMost = box.topRight; }
		if (box.bottomRight->y < bottomMost->y) { bottomMost = box.bottomRight; }

		int minX = leftMost->x;
		int maxX = rightMost->x;
		int minY = bottomMost->y;
		int maxY = topMost->y;

		int width = maxX - minX;
		int height = maxY - minY;

		int middleX = minX + width / 2;
		int middleY = minY + height / 2;

		centerPoints.push_back(new Vec2(middleX, middleY));

		// Random
		//int pointX = UtilRandom::Instance()->Random(minX, maxX);
		//int pointY = UtilRandom::Instance()->Random(minY, maxY);
		//centerPoints.push_back(new Vec2(pointX, pointY));
	}

	// Create minor roads in a raster template in each segment
	float distBetweenRoads = 15;
	float angle = 45;
	std::vector<MinorRoad> minorRoads;
	for (auto const &point : centerPoints)
	{
		angle += 5;
		//distBetweenRoads = UtilRandom::Instance()->Random(13, 20);
		//angle = UtilRandom::Instance()->Random(20, 160);

		/*
		Dir Horizontal
		*/

		// Extend a minor road from the center until it hits a major road
		MinorRoad mr = MinorRoad(point->x, point->y, angle, nullptr);
		mr.Extend(allMajorRoads);

		// Do this again, in the opposite direction
		MinorRoad mrOpposite = MinorRoad(point->x, point->y, angle + 180, nullptr);
		mrOpposite.Extend(allMajorRoads);

		// We can use the end points of these two roads to construct one road going across
		Road road = Road(mr.end, mrOpposite.end);

		// Branch left outwards from this road
		road.CreateMinorRoads(distBetweenRoads, 0, minorRoads, allMajorRoads);
		// Branch right outwards from this road
		road.CreateMinorRoads(distBetweenRoads, 1, minorRoads, allMajorRoads);

		/*
		Dir vertical
		*/

		angle += 90;

		// Extend a minor road from the center until it hits a major road
		MinorRoad mrRotated = MinorRoad(point->x, point->y, angle, nullptr);
		mrRotated.Extend(allMajorRoads);

		// Do this again, in the opposite direction
		MinorRoad mrOppositeRotated = MinorRoad(point->x, point->y, angle + 180, nullptr);
		mrOppositeRotated.Extend(allMajorRoads);

		// We can use the end points of these two roads to construct one road going across
		Road roadRotated = Road(mrRotated.end, mrOppositeRotated.end);

		// Branch left outwards from this road
		roadRotated.CreateMinorRoads(distBetweenRoads, 0, minorRoads, allMajorRoads);
		// Branch right outwards from this road
		roadRotated.CreateMinorRoads(distBetweenRoads, 1, minorRoads, allMajorRoads);

	}
}